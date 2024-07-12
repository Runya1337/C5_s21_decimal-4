#include "../s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return compare_decimals(value_1, value_2) < 0;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return compare_decimals(value_1, value_2) <= 0;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return compare_decimals(value_1, value_2) > 0;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return compare_decimals(value_1, value_2) >= 0;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  return compare_decimals(value_1, value_2) == 0;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return compare_decimals(value_1, value_2) != 0;
}

int compare_decimals(s21_decimal value_1, s21_decimal value_2) {
  int compare_result = 0;

  normalize_decimals(&value_1, &value_2);

  int sign_1 = value_1.bits[3] >> 31;
  int sign_2 = value_2.bits[3] >> 31;

  if (sign_1 != sign_2) {
    if (sign_1 > sign_2) {
      compare_result = -1;
    } else {
      compare_result = 1;
    }
  } else {
    int abs_comparison = compare_absolute_values(value_1, value_2);

    if (sign_1 == 0) {
      compare_result = abs_comparison;
    } else {
      compare_result = -abs_comparison;
    }
  }

  return compare_result;
}

void normalize_decimals(s21_decimal *value_1, s21_decimal *value_2) {
  int scale_1 = (value_1->bits[3] >> 16) & MAX_MASK;
  int scale_2 = (value_2->bits[3] >> 16) & MAX_MASK;
  int max_scale = scale_1 > scale_2 ? scale_1 : scale_2;

  while (scale_1 < max_scale) {
    multiply_by_10(value_1);
    scale_1++;
  }
  while (scale_2 < max_scale) {
    multiply_by_10(value_2);
    scale_2++;
  }
}

void multiply_by_10(s21_decimal *number) {
  unsigned long long carry_over = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long long result =
        (unsigned long long)number->bits[i] * 10 + carry_over;
    number->bits[i] = (unsigned int)(result & MAX32BIT);
    carry_over = result >> 32;
  }
  number->bits[3] += 1 << 16;
}

int compare_absolute_values(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  int break_flag = 0;

  for (int i = 2; i >= 0 && break_flag == 0; i--) {
    if (value_1.bits[i] < value_2.bits[i]) {
      result = -1;
      break_flag = 1;
    } else if (value_1.bits[i] > value_2.bits[i]) {
      result = 1;
      break_flag = 1;
    }
  }

  return result;
}