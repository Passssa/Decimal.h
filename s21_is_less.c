#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    int flag = -1;
    int overflow_check = 0;
    int bool_val = 0;
    int sign_val1 = get_sign(value_1);
    int sign_val2 = get_sign(value_2);
    if (sign_val1 == sign_val2) {
        overflow_check = set_scale(&value_1, &value_2, 0);
        for (int i = 2; i > -1; i--) {
            flag = comparison_flag(flag, &value_1, &value_2, i, overflow_check);
            if (flag != 0) break;
        }
        if (sign_val1 == 0) {
            if (flag == 1) bool_val = TRUE;
            if (flag == 2 || flag == 0) bool_val = FALSE;
        } else {
            if (flag == 2) bool_val = TRUE;
            if (flag == 1 || flag == 0) bool_val = FALSE;
        }
    } else {
        if (sign_val1 == 1) {
            bool_val = TRUE;
        } else {
            bool_val = FALSE;
        }
        if (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0) {
            if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
                bool_val = FALSE;
            }
        }
    }
    return bool_val;
}


