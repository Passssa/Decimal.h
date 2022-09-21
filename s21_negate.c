#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
    int error = 0;
    if (result == NULL) {
        error = 1;
    }
    int sign = get_sign(value);
    if (sign == 1) value.bits[3] &= ~(1 << 31);
    if (sign == 0) value.bits[3] |= 1 << 31;
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
        for (int i = 0; i < 4; i++) {
            value.bits[i] = 0;
        }
    } else {
        *result = value;
    }
    return error;
}
