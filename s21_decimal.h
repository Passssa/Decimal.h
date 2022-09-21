#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#define FALSE 0
#define TRUE 1
#define MAX_UINT_VAL 4294967295

typedef struct {
    unsigned int bits[4];
} s21_decimal;

typedef struct {
  int bits[3][4];
  int base[6];
  int temp;
  int overflow_mul1;
  int overflow_mul2;
  int overflow_sum;
} temp_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int get_sign(s21_decimal x);
void change_sign(s21_decimal *x, int sign);
unsigned int get_scale(s21_decimal *x);
int set_scale(s21_decimal *value_1, s21_decimal *value_2, int check);
int up_scale(s21_decimal *modified_val, unsigned int diff);
int up_scale_count(s21_decimal *modified_val, unsigned int diff);
void down_scale(s21_decimal *modified_val, unsigned int diff, int round_type);
int sum_count(s21_decimal *modified_val, s21_decimal addition);
int comparison_flag(int flag, s21_decimal *value_1, s21_decimal *value_2,
int i, int overflow_check);
int binary_sum(s21_decimal *value_1, s21_decimal *value_2, s21_decimal *result);
int binary_substruction(s21_decimal *value_1, s21_decimal *value_2, s21_decimal *result);
void inverse(s21_decimal *x);
void binary_division(s21_decimal dividend, s21_decimal divider, s21_decimal *result,
s21_decimal *remainder);
void count_division_res(s21_decimal *result, int bit_result);
void bankers_rounding(s21_decimal *result, s21_decimal remainder);
void math_rounding(s21_decimal *result, s21_decimal remainder);
void floor_rounding(s21_decimal *result, int sign, int check);
int check_negative_from_decimal(s21_decimal src);
int get_scale1(s21_decimal x);
int ft_mul_bit(int nb1, int nb2, int *over);
int div10_over(int nb1, int *over);
long int sum_32bit_over(int nb1, int nb2, int *over);
int sum32_3nb_over(int nb1, int nb2, int nb3, int *over);
int ft_round_dec(temp_decimal *dc, int rem);

#endif  // SRC_S21_DECIMAL_H_
