#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
    unsigned int scale = get_scale(&value);
    int sign = get_sign(value);
    if (scale == 0) {
        for (int i = 0; i < 3; i++) {
            result->bits[i] = value.bits[i];
        }
        result->bits[3] = 0;
        if (sign == 1) result->bits[3] = result->bits[3] | 2147483648;
    } else {
        down_scale(&value, scale, 1);
        for (int i = 0; i < 3; i++) {
            result->bits[i] = value.bits[i];
        }
        result->bits[3] = 0;
        if (sign == 1) result->bits[3] = result->bits[3] | 2147483648;
    }
    return 0;
}
