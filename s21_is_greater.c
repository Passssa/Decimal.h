#include "s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    int bool_val;
    bool_val = s21_is_less_or_equal(value_1, value_2);
    return (bool_val == FALSE)? TRUE : FALSE;
}
