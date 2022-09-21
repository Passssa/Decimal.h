#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  temp_decimal dc_temp = {{{0}}, {0, 0, 0, 0, 0, 0}, 0, 0, 0, 0};
  temp_decimal *p_dc_temp = &dc_temp;
  int *p_over_mul2 = &dc_temp.overflow_mul2;
  int *p_over_sum = &dc_temp.overflow_sum;
  int scale1 = get_scale1(value_1);
  int scale2 = get_scale1(value_2);
  int scale = scale1 + scale2;
  int res = 0;
  int sign = get_sign(value_1) ^ get_sign(value_2);

  int i;
  int j;
  i = 0;
  while (i < 3) {
    j = 0;
    while (j < 3) {
      dc_temp.temp = ft_mul_bit(value_1.bits[i], value_2.bits[j], p_over_mul2);
      dc_temp.temp =
          sum_32bit_over(dc_temp.temp, dc_temp.overflow_mul1, p_over_sum);
      dc_temp.bits[i][j] = dc_temp.temp;
      dc_temp.temp = 0;
      dc_temp.overflow_mul1 = dc_temp.overflow_mul2 + dc_temp.overflow_sum;
      j++;
    }
    dc_temp.temp =
        sum_32bit_over(dc_temp.temp, dc_temp.overflow_mul1, p_over_sum);
    dc_temp.bits[i][j] = dc_temp.temp;
    dc_temp.temp = 0;
    dc_temp.overflow_mul1 = dc_temp.overflow_mul2 + dc_temp.overflow_sum;
    dc_temp.overflow_mul1 = 0;
    dc_temp.overflow_mul2 = 0;
    dc_temp.overflow_sum = 0;
    i++;
  }
  dc_temp.overflow_sum = 0;
  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  int k = -3;
  while (k < 3) {
    if (k < 1) {
      num1 = dc_temp.bits[0][k + 3];
    } else {
      num1 = 0;
    }
    if (k > -3 && k < 2)
      num2 = dc_temp.bits[1][k + 2];
    else
      num2 = 0;
    if (k > -2 && k < 3)
      num3 = dc_temp.bits[2][k + 1];
    else
      num3 = 0;
    dc_temp.base[k + 3] = sum32_3nb_over(num1, num2, num3, p_over_sum);
    k++;
  }

  dc_temp.overflow_sum = 0;
  k = 0;
  while (scale > 0) {
    if (dc_temp.base[3] == 0 && dc_temp.base[4] == 0 && dc_temp.base[5] == 0) {
      if (scale < 29) {
        break;
      }
    }
    i = 5;
    dc_temp.overflow_sum = 0;
    while (i > -1) {
      dc_temp.base[i] = div10_over(dc_temp.base[i], p_over_sum);
      i--;
    }
    scale--;
  }
  dc_temp.temp = ft_round_dec(p_dc_temp, dc_temp.overflow_sum);
  if (dc_temp.temp > 2 && scale > 0) {
    k = 0;
    while (scale > 0) {
      if (dc_temp.base[3] == 0 && dc_temp.base[4] == 0 &&
          dc_temp.base[5] == 0) {
        if (scale < 29) {
          break;
        }
      }
      i = 5;
      dc_temp.overflow_sum = 0;
      while (i > -1) {
        dc_temp.base[i] = div10_over(dc_temp.base[i], p_over_sum);
        i--;
      }
      scale--;
    }
  }
  if (dc_temp.base[3] != 0 || dc_temp.base[4] != 0 || dc_temp.base[5] != 0) {
    if (sign == 1)
      res = 2;
    else
      res = 1;
  } else if (dc_temp.base[0] == 0 && dc_temp.base[1] == 0 &&
             dc_temp.base[2] == 0) {
    res = 2;
  }
  if (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0) {
    res = 0;
  } else if (value_2.bits[0] == 0 && value_2.bits[1] == 0 &&
             value_2.bits[2] == 0) {
    res = 0;
  }
  if (get_scale1(value_1) < 0 || get_scale1(value_2) < 0) res = 2;

  if (res == 0) {
    result->bits[0] = dc_temp.base[0];
    result->bits[1] = dc_temp.base[1];
    result->bits[2] = dc_temp.base[2];
    result->bits[3] = 0;
    result->bits[3] = result->bits[3] | scale << 16;
    result->bits[3] = result->bits[3] | sign << 31;
  }

  return res;
}
