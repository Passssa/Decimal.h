#include "s21_decimal.h"

int float_to_decimal_converter(char *float_to_string, s21_decimal *temp_dest,
s21_decimal *addition, int i);

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    s21_decimal temp_dest = {{0, 0, 0, 0}};
    int error_code = 0;
    char *float_to_string = NULL;
    float_to_string = malloc(10000 * sizeof(char));
    char buff[5000];
    if (float_to_string == NULL) {
        exit(1);
    } else {
        int sign = 0;
        if (src < 0) {
            sign = 1;
            src = (-1) * src;
        }
        snprintf(float_to_string, sizeof(buff), "%.50f", src);
        int i = 0;
        int flag = 0;
        while (float_to_string[i] != '.') {
            if (float_to_string[i] == '\0') break;
            i++;
        }
        if (i > 29) {
            error_code = 1;
        } else {
            int counter = i;
            i = 0;
            int check = 0;
            s21_decimal addition = {{0, 0, 0, 0}};
            while (float_to_string[i] != '.') {
                unsigned int temp = float_to_string[i] - '0';
                if (i > 6) temp = 0;
                addition.bits[0] = temp;
                flag = sum_count(&temp_dest, addition);
                if (i == 6) {
                    s21_decimal remainder = {{0, 0, 0, 0}};
                    if (float_to_string[i+1] != '.') {
                        remainder.bits[0] = float_to_string[i+1] - '0';
                    } else {
                        remainder.bits[0] = float_to_string[i+2] - '0';
                    }
                    math_rounding(&temp_dest, remainder);
                }
                if (i < counter-1) check = up_scale(&temp_dest, 1);
                if (flag == -1 || check == -1) {
                    error_code = 1;
                    break;
                }
                addition.bits[0] = 0;
                i++;
            }
            if (error_code != 1) {
                int k = -1;
                i = i + 1;
                temp_dest.bits[3] = 0;
                if (i < 8) k = float_to_decimal_converter(float_to_string, &temp_dest, &addition, i);

                if (sign == 1) temp_dest.bits[3] = temp_dest.bits[3] | 2147483648;
                for (int i = 0; i < 4; i++) {
                    dst->bits[i] = temp_dest.bits[i];
                }
                if (((k > 7 && temp_dest.bits[0] == 0) || k == -1) && src < 1.0) {
                    for (int i = 0; i < 4; i++) {
                        dst->bits[i] = 0;
                    }
                    if (k > 28) error_code = 1;
                }
            }
        }
        free(float_to_string);
    }
    return error_code;
}

int float_to_decimal_converter(char *float_to_string, s21_decimal *temp_dest,
s21_decimal *addition, int i) {
        s21_decimal temp_dest2 = {{0, 0, 0, 0}};
        s21_decimal addition_temp = {{0, 0, 0, 0}};
        int counter = 1;
        int check = 0;
        int check_2 = 0;
        int flag = 0;
        int k = -1;
        while (counter < 39 && check != -1 && flag != -1) {
            temp_dest2 = *temp_dest;
            unsigned int temp = float_to_string[i] - '0';
            addition->bits[0] = temp;
            addition_temp.bits[0] = addition->bits[0];
            if ((temp_dest->bits[0] != 0 || temp_dest->bits[1] != 0 ||
            temp_dest->bits[2] != 0) && check_2 == 0) {
                if (i < 8) {
                    check = up_scale(temp_dest, 1);
                    if (check != -1) {
                        flag = sum_count(temp_dest, *addition);
                    }
                } else if (i == 8) {
                    math_rounding(temp_dest, *addition);
                } else if (i > 8) {
                    break;
                }
            } else if ((temp_dest->bits[0] == 0 && temp_dest->bits[1] == 0 &&
            temp_dest->bits[2] == 0) || check_2 == 1) {
                check_2 = 1;
                if (temp != 0 && k == -1) k = counter;
                if (counter < 8) {
                    up_scale(temp_dest, 1);
                    sum_count(temp_dest, *addition);
                }
                if (k != -1 && counter == 29) {
                    for (int i = 0; i < 4; i++) {
                        temp_dest->bits[i] = 0;
                    }
                    i = 9;
                    check_2 = -1;
                }
                if (k != -1 && k < 9 && counter == 8) {
                    math_rounding(temp_dest, *addition);
                    i = 9;
                    check_2 = 0;
                }
            }
            i++;
            counter++;
        }
        if (check == -1 || flag == -1) {
            *temp_dest = temp_dest2;
            addition->bits[0] = addition_temp.bits[0];
            bankers_rounding(temp_dest, *addition);
        }
        temp_dest->bits[3] = 0;
        if (check_2 == 0) {
            temp_dest->bits[3] = temp_dest->bits[3] + (counter - 2) * 65536;
        } else if (check_2 == 1) {
            temp_dest->bits[3] = temp_dest->bits[3] + 28 * 65536;
        }
    return k;
}
