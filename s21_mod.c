#include "s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    for (int i = 0; i < 4; i++) {
        result->bits[i] = 0;
    }
    int sign_val1 = get_sign(value_1);
    int sign_val2 = get_sign(value_2);
    int set_minus = 0;
    set_scale(&value_1, &value_2, 1);
    if ((sign_val1 == 1 && sign_val2 == 1) || (sign_val1 == 1 && sign_val2 == 0))
    set_minus = 1;
    s21_decimal quotient = {{0, 0, 0, 0}};
    value_1.bits[3] = 0;
    value_2.bits[3] = 0;
    binary_division(value_1, value_2, &quotient, result);
    result->bits[3] = 0;
    if (set_minus == 1) change_sign(result, 0);
    return 0;
}
