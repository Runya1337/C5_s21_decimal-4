#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  unsigned meta = (getSign(*result) << 31) + (getScale(*result) << 16);
  s21_decimal c = {{0, 0, 0, meta}, 0};
  *result = c;
  int sign_value1 = getSign(value_1);
  int sign_value2 = getSign(value_2);
  int reminder = 0;
  if (!getScale(*result)) {
    normalize_decimals(&value_1, &value_2);
    setScale(result, getScale(value_1));
  }
  if (helper_add(sign_value1, sign_value2, &value_1, &value_2, result) == 0) {
    s21_sub(value_1, value_2, result);
  } else {
    for (int bit = 0; bit < 3; bit++) {
      for (int position = 0; position < 32; position++) {
        int bit_1 = getBit(value_1.bits[bit], position);
        int bit_2 = getBit(value_2.bits[bit], position);
        checkoverflow_for_add(bit_1, bit_2, reminder, position, bit, result);
        if (reminder & (bit_1 & bit_2)) {
          setBit(&result->bits[bit], position);
          reminder = 1;
        } else if (reminder & ((!bit_1) & (!bit_2))) {
          setBit(&result->bits[bit], position);
          reminder = 0;
        } else if (reminder & (bit_1 ^ bit_2)) {
          reminder = 1;
        } else if ((!reminder) & (bit_1 & bit_2)) {
          reminder = 1;
        } else if ((!reminder) & (bit_1 ^ bit_2)) {
          setBit(&result->bits[bit], position);
        }
        check_overflow_add(position, bit, bit_1, bit_2, reminder, result);
      }
    }
  }
  return 0;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_value1 = getSign(value_1);
  int sign_value2 = getSign(value_2);
  int take = 0;
  normalize_decimals(&value_1, &value_2);
  setScale(result, getScale(value_1));
  if (helper_sub(sign_value1, sign_value2, &value_1, &value_2, result) == 0) {
    s21_add(value_1, value_2, result);
  } else {
    for (int bit = 0; bit < 3; bit++) {
      for (int position = 0; position < 32; position++) {
        int bit_1 = getBit(value_1.bits[bit], position);
        int bit_2 = getBit(value_2.bits[bit], position);
        if (take & (bit_1 & bit_2)) {
          setBit(&result->bits[bit], position);
        } else if (take & (bit_1 & !bit_2)) {
          take = 0;
        } else if (take & ((!bit_1) & bit_2)) {
        } else if ((!take) & (bit_1 & !bit_2)) {
          setBit(&result->bits[bit], position);
        } else if ((!take) & ((!bit_1) & bit_2)) {
          take = 1;
          setBit(&result->bits[bit], position);
        } else if (take & (!bit_1 & !bit_2)) {
          setBit(&result->bits[bit], position);
        }
      }
    }
  }
  return 0;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_value1 = getSign(value_1);
  int sign_value2 = getSign(value_2);
  int allposition;
  s21_decimal pre_mul = {{0, 0, 0, 0}, 0};
  if (sign_value1 ^ sign_value2) {
    s21_negate(*result, result);
  }
  int result_scale = getScale(value_1) + getScale(value_2);
  if (result_scale > 29) {
    result->value_type = DECIMAL_OVERFLOW;
  } else {
    setScale(result, result_scale);
    destroyScale(&value_1);
    destroyScale(&value_2);
    setPlus(&value_1);
    setPlus(&value_2);
    for (int bit = 0; bit < 3; bit++) {
      for (int position = 0; position < 32; position++) {
        if (getBit(value_2.bits[bit], position) == 1) {
          allposition = 32 * bit + position;
          pre_mul = pow_2_decimal(value_1, allposition);
          s21_add(pre_mul, *result, result);
        }
      }
    }
  }
  return 0;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_add(value_1, value_2, result);
  return 0;
}