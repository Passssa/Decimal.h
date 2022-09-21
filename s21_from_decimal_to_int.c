#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int result = 0;
    int error = 0;
    s21_decimal src0 = src;
    if ((src.bits[3] & 2031616) != 0) {
        if (s21_truncate(src0, &src)) {
            error = 1;
        }
    }
    if (src.bits[2] != 0 || src.bits[1] != 0 || src.bits[0] > INT_MAX) {
        error = 1;
    } else {
        if (src.bits[3] == 0 || src.bits[3] == 2147483648) {
            int sign = check_negative_from_decimal(src);
            if (sign == 1 || sign == -1) {
                for (int i = 0; i < 32; i++) {
                    result += (src.bits[0] & (int)pow(2, i));
                }
                *dst = result * sign;
            } else {
                error = 1;
            }
        }
    }
    return error;
}
