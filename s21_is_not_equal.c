#include "s21_decimal.h"

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    int bool_val = FALSE;
    if (s21_is_less(value_1, value_2) == 1 ||
    s21_is_greater(value_1, value_2) == 1) bool_val = TRUE;
    return bool_val;
}
