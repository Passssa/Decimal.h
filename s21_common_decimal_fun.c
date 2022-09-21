#include "s21_decimal.h"

int get_sign(s21_decimal x) {
    unsigned int sign;
    unsigned int mask = 2147483648;
    sign = x.bits[3] & mask;
    return (sign != 0)? 1 : 0;
}

void change_sign(s21_decimal *x, int sign) {
    if (sign == 0) {
        unsigned int mask = 2147483648;
        x->bits[3] = x->bits[3] | mask;
    } else {
        unsigned int mask = 2147483647;
        x->bits[3] = x->bits[3] & mask;
    }
}

unsigned int get_scale(s21_decimal *x) {
    unsigned int scale;
    unsigned int mask = 16711680;
    scale = x->bits[3] & mask;
    if (scale%65536 != 0 || scale/65536 > 28) {
        exit(1);
    } else {
        scale = scale/65536;
    }
    return scale;
}

int binary_sum(s21_decimal *value_1, s21_decimal *value_2, s21_decimal *result) {
    int temp_buff = 0;
    for (int i = 0; i < 4; i++) {
        result->bits[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
        result->bits[i] = value_1->bits[i];
    }
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 32; k++) {
            int counter = 0;
            unsigned int bit_1 = value_1->bits[i];
            unsigned int bit_2 = value_2->bits[i];
            unsigned int mask = 1 << k;
            bit_1 = bit_1 & mask;
            bit_2 = bit_2 & mask;
            int result_1 = (bit_1 == mask)? 1 : 0;
            int result_2 = (bit_2 == mask)? 1 : 0;
            if (result_1 == 1) counter++;
            if (result_2 == 1) counter++;
            if (temp_buff == 1) counter++;
            if (counter == 0) {
                result->bits[i] = result->bits[i] & (~mask);
                temp_buff = 0;
            } else if (counter == 1) {
                result->bits[i] = result->bits[i] | mask;
                temp_buff = 0;
            } else if (counter == 2) {
                result->bits[i] = result->bits[i] & (~mask);
                temp_buff = 1;
            } else if (counter == 3) {
                result->bits[i] = result->bits[i] | mask;
                temp_buff = 1;
            }
        }
    }
    return temp_buff;
}

int binary_substruction(s21_decimal *value_1, s21_decimal *value_2, s21_decimal *result) {
    inverse(value_2);
    binary_sum(value_1, value_2, result);
    return 0;
}

void inverse(s21_decimal *x) {
    for (int i = 0; i < 3; i++) {
        x->bits[i] = ~x->bits[i];
    }
    s21_decimal inversed_value = {{x->bits[0], x->bits[1],
    x->bits[2], x->bits[3]}};
    s21_decimal add = {{1, 0, 0, 0}};
    binary_sum(x, &add, &inversed_value);
    for (int i = 0; i < 3; i++) {
        x->bits[i] = inversed_value.bits[i];
    }
}

void binary_division(s21_decimal dividend, s21_decimal divider, s21_decimal *result, s21_decimal *remainder) {
    for (int i = 0; i < 4; i++) {
        result->bits[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        remainder->bits[i] = 0;
    }
    s21_decimal helper_dividend = {{0, 0, 0, 0}};
    s21_decimal result_of_sub = {{0, 0, 0, 0}};
    s21_decimal divider_temp = {{divider.bits[0], divider.bits[1], divider.bits[2], divider.bits[3]}};
    int check_1 = 0;
    int check_2 = 0;
    for (int i = 2; i > -1; i--) {
        for (int k = 31; k > -1; k--) {
            int bit_result = 0;
            unsigned int mask = 1 << k;
            unsigned int dividend_bit = dividend.bits[i] & mask;
            if (dividend_bit != 0) {
                bit_result = 1;
                check_1 = 1;
            }
            if (check_1 == 1) {
                count_division_res(&helper_dividend, bit_result);
            }
            if (s21_is_greater(divider, helper_dividend) == 1 && check_1 == 1) {
                if (check_2 == 1) {
                    count_division_res(result, 0);
                }
            } else if (s21_is_greater(divider, helper_dividend) == 0 && check_1 == 1) {
                binary_substruction(&helper_dividend, &divider_temp, &result_of_sub);
                for (int i = 0; i < 3; i++) {
                    helper_dividend.bits[i] = result_of_sub.bits[i];
                }
                for (int i = 0; i < 3; i++) {
                    divider_temp.bits[i] = divider.bits[i];
                }
                for (int i = 0; i < 3; i++) {
                    result_of_sub.bits[i] = 0;
                }
                count_division_res(result, 1);
                check_2 = 1;
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        remainder->bits[i] = helper_dividend.bits[i];
    }
}

void count_division_res(s21_decimal *result, int bit_result) {
    unsigned int mask = 1 << 31;
    int overflow_bit0 = 0;
    int overflow_bit1 = 0;
    if ((result->bits[0] & mask) == 2147483648) overflow_bit0 = 1;
    if ((result->bits[1] & mask) == 2147483648) overflow_bit1 = 1;
    for (int i = 0; i < 3; i++) {
        result->bits[i] = result->bits[i] << 1;
    }
    result->bits[0] = result->bits[0] | bit_result;
    if (overflow_bit0 == 1) result->bits[1] = result->bits[1] | 1;
    if (overflow_bit1 == 1) result->bits[2] = result->bits[2] | 1;
}

int sum_count(s21_decimal *modified_val, s21_decimal addition) {
    int flag = 1;
    int overflow = 0;
    int overflow_2 = 0;
        if (MAX_UINT_VAL - modified_val->bits[0] >= addition.bits[0]) {
            modified_val->bits[0] = addition.bits[0] + modified_val->bits[0];
        } else {
            modified_val->bits[0] = modified_val->bits[0] - (MAX_UINT_VAL - addition.bits[0] + 1);
            overflow = 1;
        }
        if (MAX_UINT_VAL - modified_val->bits[1] >= addition.bits[1]) {
            modified_val->bits[1] = addition.bits[1] + modified_val->bits[1];
            if (overflow == 1) {
                if (MAX_UINT_VAL - modified_val->bits[1] >= 1) {
                    modified_val->bits[1]++;
                } else {
                    modified_val->bits[1] = 0;
                    overflow_2 = 1;
                }
            }
            if (MAX_UINT_VAL - modified_val->bits[2] >= addition.bits[2]) {
                modified_val->bits[2] = addition.bits[2] + modified_val->bits[2];
                if (overflow_2 == 1) {
                    if (MAX_UINT_VAL - modified_val->bits[2] >= 1) {
                        modified_val->bits[2]++;
                    } else {
                        modified_val->bits[2] = 0;
                        flag = -1;
                    }
                }
            } else {
                modified_val->bits[2] = MAX_UINT_VAL;
                flag = -1;
            }
        } else {
            modified_val->bits[1] = modified_val->bits[1] - (MAX_UINT_VAL - addition.bits[1] + 1);
            if (overflow == 1) {
                if (MAX_UINT_VAL - modified_val->bits[1] >= 1) {
                    modified_val->bits[1]++;
                } else {
                    modified_val->bits[1] = 0;
                    overflow_2 = 1;
                }
            }
            if (MAX_UINT_VAL - (modified_val->bits[2]) >= addition.bits[2]) {
                modified_val->bits[2] = addition.bits[2] + modified_val->bits[2];
                modified_val->bits[2]++;
                if (overflow_2 == 1) {
                    if (MAX_UINT_VAL - modified_val->bits[2] >= 1) {
                        modified_val->bits[2]++;
                    } else {
                        modified_val->bits[2] = 0;
                        flag = -1;
                    }
                }
            } else {
                modified_val->bits[2] = MAX_UINT_VAL;
                flag = -1;
            }
        }
    return flag;
}

int set_scale(s21_decimal *value_1, s21_decimal *value_2, int check) {
    int flag = 0;
    unsigned int diff = 0;
    s21_decimal *modified_val_up = NULL;
    s21_decimal *modified_val_down = NULL;
    unsigned int scale_val1 = get_scale(value_1);
    unsigned int scale_val2 = get_scale(value_2);
    if (scale_val1 != scale_val2) {
        if (scale_val1 > scale_val2) {
            diff = scale_val1 - scale_val2;
            modified_val_up = value_2;
        } else {
            diff = scale_val2 - scale_val1;
            modified_val_up = value_1;
        }
        flag = up_scale(modified_val_up, diff);
        scale_val1 = get_scale(value_1);
        scale_val2 = get_scale(value_2);
        if (scale_val1 != scale_val2 && check == 1) {
            if (modified_val_up == value_1) {
                diff = scale_val2 - scale_val1;
                modified_val_down = value_2;
            } else if (modified_val_up == value_2) {
                diff = scale_val1 - scale_val2;
                modified_val_down = value_1;
            }
            down_scale(modified_val_down, diff, 0);
        }
    }
    if (flag == -1 && modified_val_up == value_2) flag = -2;
    return flag;
}

int up_scale(s21_decimal *modified_val, unsigned int diff) {
    int flag = 0;
    unsigned int diff_next = 0;
    while (diff > 0 && flag != -1) {
        if (diff < 5) {
            flag = up_scale_count(modified_val, diff);
            break;
        } else {
            diff_next = 4;
            diff = diff - diff_next;
            flag = up_scale_count(modified_val, diff_next);
        }
    }
    return flag;
}

int up_scale_count(s21_decimal *modified_val, unsigned int diff) {
    int flag = 1;
    unsigned int scale_num = 1;
    s21_decimal addition = {{modified_val->bits[0], modified_val->bits[1],
    modified_val->bits[2], modified_val->bits[3]}};
    s21_decimal temp = {{modified_val->bits[0], modified_val->bits[1],
    modified_val->bits[2], modified_val->bits[3]}};
    for (unsigned long long int i = 1; i < pow(10, diff); i++) {
        flag = sum_count(modified_val, addition);
        if (flag == -1) break;
        if (pow((i + 1), scale_num) == pow(10, scale_num)) {
            temp.bits[0] = modified_val->bits[0];
            temp.bits[1] = modified_val->bits[1];
            temp.bits[2] = modified_val->bits[2];
            scale_num++;
        }
    }
    if (flag == -1) {
        modified_val->bits[0] = temp.bits[0];
        modified_val->bits[1] = temp.bits[1];
        modified_val->bits[2] = temp.bits[2];
        scale_num--;
        modified_val->bits[3] = modified_val->bits[3] + scale_num * 65536;
    } else {
        modified_val->bits[3] = modified_val->bits[3] + diff * 65536;
    }
    return flag;
}

void down_scale(s21_decimal *modified_val, unsigned int diff, int round_type) {
    unsigned int old_scale = (modified_val->bits[3] & 2147483647)/65536;
    unsigned int new_scale = old_scale - diff;
    int sign = get_sign(*modified_val);
    modified_val->bits[3] = 0;
    s21_decimal divider_1 = {{1, 0, 0, 0}};
    s21_decimal divider_2 = {{1, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal result_temp = {{0, 0, 0, 0}};
    s21_decimal remainder_1 = {{0, 0, 0, 0}};
    s21_decimal remainder_2 = {{0, 0, 0, 0}};
    up_scale(&divider_1, diff);
    up_scale(&divider_2, diff-1);
    divider_1.bits[3] = 0;
    divider_2.bits[3] = 0;
    binary_division(*modified_val, divider_1, &result, &remainder_1);
    binary_division(*modified_val, divider_2, &result_temp, &remainder_2);
    s21_decimal scaled_result_1 = result;
    up_scale(&scaled_result_1, 1);
    s21_decimal result_remainder = {{0, 0, 0, 0}};
    binary_substruction(&result_temp, &scaled_result_1, &result_remainder);
    if (round_type == 1) {
        math_rounding(&result, result_remainder);
    } else if (round_type == 2) {
        int check = 0;
        if (remainder_1.bits[0] == 0 && remainder_1.bits[1] == 0 &&
        remainder_1.bits[2] == 0) check = 1;
        floor_rounding(&result, sign, check);
    } else if (round_type == 0) {
        bankers_rounding(&result, result_remainder);
    }
    for (int i = 0; i < 3; i++) {
        modified_val->bits[i] = result.bits[i];
    }
    if (sign == 1) modified_val->bits[3] = modified_val->bits[3] & 2147483648;
    modified_val->bits[3] = modified_val->bits[3] + new_scale * 65536;
}

void bankers_rounding(s21_decimal *result, s21_decimal remainder) {
    s21_decimal addition = {{1, 0, 0, 0}};
    if (remainder.bits[0] > 5) {
        sum_count(result, addition);
    } else if (remainder.bits[0] == 5) {
        if ((result->bits[0] & 1) == 1) {
            sum_count(result, addition);
        }
    }
}

void math_rounding(s21_decimal *result, s21_decimal remainder) {
    s21_decimal addition = {{1, 0, 0, 0}};
    if (remainder.bits[0] >= 5) {
        sum_count(result, addition);
    }
}

void floor_rounding(s21_decimal *result, int sign, int check) {
    s21_decimal addition = {{1, 0, 0, 0}};
    if (sign == 1) {
        if (check != 1) sum_count(result, addition);
    }
}

int comparison_flag(int flag, s21_decimal *value_1, s21_decimal *value_2,
int i, int overflow_check) {
    flag = 0;
    if (value_1->bits[i] == value_2->bits[i]) {
        flag = 0;
    } else if (value_1->bits[i] < value_2->bits[i]) {
        flag = 1;
    } else if (value_1->bits[i] > value_2->bits[i]) {
        flag = 2;
    }
    if (overflow_check == -1) flag = 2;
    if (overflow_check == -2) flag = 1;
    return flag;
}

int check_negative_from_decimal(s21_decimal src) {
    int sign = 1;
    unsigned int mask = 0;
    for (int i = 0; i < 16; i++) {
        mask = 1 << i;
        if (src.bits[3] & mask) {
            sign = 0;
        }
    }
    for (int i = 21; i < 31; i++) {
        mask = 1 << i;
        if (src.bits[3] & mask) {
            sign = 0;
        }
    }
    mask = 1 << 31;
    if (src.bits[3] & mask) {
        sign = -1;
    }
    return sign;
}

int get_scale1(s21_decimal x) {
  int sc = x.bits[3];
  sc = sc >> 16;
  sc = sc & ~(1 << 15);
  sc = sc & 0xFF;
  if (sc > 28) sc = -1;
  return sc;
}

int ft_mul_bit(int nb1, int nb2, int *over) {
  long int val1;
  long int val2;
  long int temp1 = nb1;
  long int temp2 = nb2;
  val1 = temp1 & 0xFFFFFFFF;
  val2 = temp2 & 0xFFFFFFFF;

  long int result = 0;
  while (val2 != 0) {
    if ((val2 & 0x1) == 0x1) result += val1;

    val2 >>= 1;
    val1 <<= 1;
  }
  *over = result >> 32;
  return result;
}

int div10_over(int nb1, int *over) {
  long int quot;
  long int dividend = nb1 & 0xFFFFFFFF;
  long int rem = ((long int)*over << 32);
  dividend += rem;
  if (dividend / 10 > 0) {
    quot = dividend / 10;
  } else {
    quot = 0;
  }
  *over = dividend % 10;
  return quot;
}

long int sum_32bit_over(int nb1, int nb2, int *over) {
  long int sum;
  long int nbl2;
  long int temp1 = nb1;
  long int temp2 = nb2;
  sum = temp1 & 0xFFFFFFFF;
  nbl2 = temp2 & 0xFFFFFFFF;
  long int carry = 0;
  while (nbl2 != 0) {
    carry = sum & nbl2;
    sum = sum ^ nbl2;
    nbl2 = carry << 1;
  }
  *over = sum >> 32;
  return sum;
}

int sum32_3nb_over(int nb1, int nb2, int nb3, int *over) {
  long int sum;
  long int temp1 = nb1 & 0xFFFFFFFF;
  long int temp2 = nb2 & 0xFFFFFFFF;
  long int temp3 = nb3 & 0xFFFFFFFF;
  long int temp4 = *over & 0xFFFFFFFF;
  sum = temp1 + temp2 + temp3 + temp4;
  *over = sum >> 32;
  return sum;
}

int ft_round_dec(temp_decimal *dc, int rem) {
  int res = 0;
  int i = 0;
  long int temp;
  temp = dc->base[0] & 0xFFFFFFFF;
  if (rem == 5) {
    if (res & 1) {
      temp += 1;
    }
  } else if (rem > 5) {
    temp += 1;
  }
  while (i < 6) {
    dc->base[i] = temp;
    if (temp > MAX_UINT_VAL) {
      temp = dc->base[i + 1] & 0xFFFFFFFF;
    } else {
      break;
    }
    temp += 1;
    i++;
  }
  return res = i;
}
