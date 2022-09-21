#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    for (int i = 0; i < 4; i++) {
        result->bits[i] = 0;
    }
    int sign_val1 = get_sign(value_1);
    int sign_val2 = get_sign(value_2);
    int set_minus = 0;
    int error_code = 0;
    if ((sign_val1 == 1 && sign_val2 == 0) || (sign_val1 == 0 && sign_val2 == 1)) {
        change_sign(&value_2, sign_val2);
        error_code = s21_add(value_1, value_2, result);
    } else {
        if (sign_val1 == 1 && sign_val2 == 1) {
            s21_decimal temp = {{0, 0, 0, 0}};
            change_sign(&value_1, sign_val1);
            change_sign(&value_2, sign_val2);
            temp = value_1;
            value_1 = value_2;
            value_2 = temp;
        }
        set_scale(&value_1, &value_2, 1);
        if (s21_is_greater_or_equal(value_1, value_2) != 1) {
            s21_decimal temp = {{0, 0, 0, 0}};
            temp = value_1;
            value_1 = value_2;
            value_2 = temp;
            set_minus = 1;
        }
        binary_substruction(&value_1, &value_2, result);
        result->bits[3] = 0;
        unsigned int result_scale = get_scale(&value_1);
        result->bits[3] = result->bits[3] + result_scale * 65536;
        if (set_minus == 1) change_sign(result, 0);
    }
    return error_code;
}
