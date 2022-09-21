#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    for (int i = 0; i < 4; i++) {
        result->bits[i] = 0;
    }
    int error_code = 0;
    int set_minus = 0;
    int sign_val1 = get_sign(value_1);
    int sign_val2 = get_sign(value_2);
    if ((sign_val1 == 1 && sign_val2 == 0) || (sign_val1 == 0 && sign_val2 == 1)) {
        if (sign_val2 == 1) change_sign(&value_2, sign_val2);
        if (sign_val1 == 1) {
            change_sign(&value_1, sign_val1);
            s21_decimal temp = {{0, 0, 0, 0}};
            temp = value_1;
            value_1 = value_2;
            value_2 = temp;
        }
        error_code = s21_sub(value_1, value_2, result);
    } else {
        if (sign_val1 == 1 && sign_val2 == 1) set_minus = 1;
        set_scale(&value_1, &value_2, 1);
        unsigned int result_scale = get_scale(&value_1);
        s21_decimal result_temp = {{0, 0, 0, 0}};
        s21_decimal value_1_temp = value_1;
        s21_decimal value_2_temp = value_2;
        int buff = binary_sum(&value_1_temp, &value_2_temp, &result_temp);
        if (buff != 0 && result_scale == 0) {
            if (set_minus == 0) error_code = 1;
            if (set_minus == 1) error_code = 2;
        } else if ((buff != 0 && result_scale > 0)) {
            result_scale--;
            s21_decimal value_1_temp = value_1;
            s21_decimal value_2_temp = value_2;
            s21_decimal remainder1 = {{0, 0, 0, 0}};
            s21_decimal remainder2 = {{0, 0, 0, 0}};
            s21_decimal divider = {{10, 0, 0, 0}};
            value_1.bits[3] = 0;
            binary_division(value_1, divider, &value_1_temp, &remainder1);
            binary_division(value_2, divider, &value_2_temp, &remainder2);
            s21_decimal remainder_all = {{0, 0, 0, 0}};
            binary_sum(&remainder1, &remainder2, &remainder_all);
            binary_sum(&value_1_temp, &value_2_temp, &result_temp);
            s21_decimal example = {{10, 0, 0, 0}};
            s21_decimal compar_val = {{1, 0, 0, 0}};
            if (s21_is_greater_or_equal(remainder_all, example)) {
                sum_count(&result_temp, compar_val);
                remainder_all.bits[3] = 0;
                s21_sub(remainder_all, example, &remainder_all);
            }
            bankers_rounding(&result_temp, remainder_all);
        }
        if (error_code == 0) {
            for (int i = 0; i < 3; i++) {
                result->bits[i] = result_temp.bits[i];
            }
            result->bits[3] = 0;
            result->bits[3] = result_scale * 65536;
            if (set_minus == 1) change_sign(result, 0);
        }
    }
    return error_code;
}
