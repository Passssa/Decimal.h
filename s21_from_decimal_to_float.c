#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    double dest_buf = 0;
    int counter = 95;
    int sign = get_sign(src);
    unsigned int scale = get_scale(&src);
    for (int i = 2; i > -1; i--) {
        for (int k = 31; k >-1; k--) {
            unsigned int mask = 1 << k;
            if ((src.bits[i] & mask) != 0) dest_buf = dest_buf + pow(2, counter);
            counter--;
        }
    }
    dest_buf = dest_buf / pow(10, scale);
    if (sign == 1) dest_buf = (-1) * dest_buf;
    *dst = (float)dest_buf;
    return 0;
}
