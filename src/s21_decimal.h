#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MASK 255
#define MIN_MASK 0b01111111111111111111111111111111
#define DESTROY_SCALE 0b10000000000000000000000000000000
#define MAX32BIT 0xFFFFFFFF
#define CHANGE_SIGN 0x80000000
#define S21_INF 1.0 / 0.0
#define MASK1111 0b11111111111111111111111111111111
#define S21_NAN 0.0 / 0.0

typedef enum value_type {
  DECIMAL_OK = 0,
  DECIMAL_OVERFLOW = 1,
  DECIMAL_UNDERFLOW = 2,
  DECIMAL_DIVISION_BY_ZERO = 3,
  ADDITIONAL_CODE = 4
} value_type_t;

typedef struct {
  unsigned int bits[4];
  value_type_t value_type;
} s21_decimal;

typedef struct {
  unsigned int bits[7];
  value_type_t value_type;
} big_decimal;

typedef enum {
  CONVERTATION_OK = 0,
  CONVERTATION_ERROR = 1
} convertation_result;

int getSign(s21_decimal value);
int s21_negate(s21_decimal value, s21_decimal *result);
int getBit(unsigned value, int position);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int getScale(s21_decimal value);
int getMask(int position);
void setScale(s21_decimal *result, unsigned scale);
void destroyScale(s21_decimal *result);
void setBit(unsigned *value, int position);
void setUnBit(unsigned *value, int position);
void setPlus(s21_decimal *result);
s21_decimal pow_2_decimal(s21_decimal value, int scale);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int compare_decimals(s21_decimal value_1, s21_decimal value_2);
void normalize_decimals(s21_decimal *value_1, s21_decimal *value_2);
void multiply_by_10(s21_decimal *number);
int compare_absolute_values(s21_decimal value_1, s21_decimal value_2);
void demical_print(s21_decimal value);
void checkoverflow_for_add(int bit_1, int bit_2, int reminder, int position,
                           int bit, s21_decimal *result);
int helper_sub(int sign_value1, int sign_value2, s21_decimal *value_1,
               s21_decimal *value_2, s21_decimal *result);
void is_a_less_b_sub(s21_decimal *value_1, s21_decimal *value_2,
                     s21_decimal *result);
int helper_add(int sign_value1, int sign_value2, s21_decimal *value_1,
               s21_decimal *value_2, s21_decimal *result);
void change_places_value(s21_decimal *value_1, s21_decimal *value_2);
void check_overflow_add(int position, int bit, int bit_1, int bit_2,
                        int reminder, s21_decimal *result);
void pow_2_decimal_helper(s21_decimal *value, int scale);

int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
void add_one(s21_decimal *value);
int divide_by_10(s21_decimal *result);
void set_sign(s21_decimal *val, int sign);
void set_sign_minus(s21_decimal *val);
void set_sign_plus(s21_decimal *val);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int get_bit(s21_decimal value, int pos);
void set_sign_minus(s21_decimal *val);
void set_sign_plus(s21_decimal *val);
void set_sign(s21_decimal *val, int sign);
void s21_set_bit(s21_decimal *number, int bit, int value);

#endif
