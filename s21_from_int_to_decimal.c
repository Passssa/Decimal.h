#include "s21_decimal.h"

int check_negative_from_int_float(int value_int, double value_float);

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    if (!dst) {
        return 1;
    } else {
        memset(dst, 0, sizeof(unsigned int) * 4);
        if (check_negative_from_int_float(src, 3)) {
            dst->bits[3] += 2147483648;
            src *= -1;
        }
        dst->bits[0] = (int)src;
        return 0;
    }
}

int check_negative_from_int_float(int value_int, double value_float) {
    int flag = 0;
    if (value_int < 0) flag = 1;
    if (value_float < 0) flag = 1;
    return flag;
}
