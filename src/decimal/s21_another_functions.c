#include "../s21_decimal.h"

int getSign(s21_decimal value) { return (value.bits[3] >> 31); }

int s21_negate(s21_decimal value, s21_decimal *result) {
  value.bits[3] ^= 0x80000000;
  *result = value;
  return 0;
}

void setPlus(s21_decimal *result) {
  int sign = getSign(*result);
  if (sign) {
    result->bits[3] = result->bits[3] & MIN_MASK;
  }
}

int getBit(unsigned value, int position) {
  return (((1 << position) & value) >> position);
}

void setBit(unsigned *value, int position) {
  *value = *value | getMask(position);
}

void setUnBit(unsigned *value, int position) {
  *value = getMask(position) ^ *value;
}

int getMask(int position) { return (1 << position); }

int getScale(s21_decimal value) {
  s21_decimal donor = value;
  setPlus(&donor);
  return (donor.bits[3] >> 16);
}

void setScale(s21_decimal *result, unsigned scale) {
  for (int i = 0; i < 8; i++) {
    if (getBit(scale, i)) {
      setBit(&result->bits[3], (16 + i));
    }
  }
}

void destroyScale(s21_decimal *result) {
  result->bits[3] = result->bits[3] & DESTROY_SCALE;
}

s21_decimal pow_2_decimal(s21_decimal value, int scale) {
  int new_position_bit;
  s21_decimal result = {{0, 0, 0, 0}, 0};
  for (int bit = 2; bit >= 0; bit--) {
    for (int position = 31; position >= 0; position--) {
      if (getBit(value.bits[bit], position) == 1) {
        new_position_bit = 32 * bit + position + scale;
        setBit(&result.bits[new_position_bit / 32], new_position_bit % 32);
      }
    }
  }
  return result;
}

void pow_2_decimal_helper(s21_decimal *value, int scale) {
  int lenght_for_first_bit = 0;
  int flag = 1;
  for (int bit = 2; bit >= 0; bit--) {
    for (int position = 31; position >= 0; position--) {
      if (getBit(value->bits[bit], position) == 1) {
        flag = 0;
        break;
      } else {
        lenght_for_first_bit = lenght_for_first_bit + 1;
      }
    }
    if (flag == 0) {
      break;
    }
  }
  if (lenght_for_first_bit < scale) {
    value->value_type = DECIMAL_OVERFLOW;
  }
}

void demical_print(s21_decimal value) {
  for (int bit = 3; bit >= 0; bit--) {
    for (int position = 31; position >= 0; position--) {
      printf("%d", getBit(value.bits[bit], position));
    }
    printf("    ");
  }
  printf("\n");
}

void checkoverflow_for_add(int bit_1, int bit_2, int reminder, int position,
                           int bit, s21_decimal *result) {
  if ((position == 31) & (bit == 2)) {
    if ((bit_1 + bit_2 + reminder) >= 2) {
      result->value_type = DECIMAL_OVERFLOW;
    }
  }
}

int helper_sub(int sign_value1, int sign_value2, s21_decimal *value_1,
               s21_decimal *value_2, s21_decimal *result) {
  int flag = 1;
  if ((sign_value1 == 1) && (sign_value2 == 1)) {
    s21_negate(*value_1, value_1);
    s21_negate(*value_2, value_2);
    s21_negate(*result, result);
    is_a_less_b_sub(value_1, value_2, result);
  }
  if ((sign_value1 == 1) && (sign_value2 == 0)) {
    s21_negate(*value_1, value_1);
    s21_negate(*result, result);
    flag = 0;
  }
  if ((sign_value1 == 0) && (sign_value2 == 1)) {
    s21_negate(*value_2, value_2);
    flag = 0;
  }
  if ((sign_value1 == 0) && (sign_value2 == 0)) {
    is_a_less_b_sub(value_1, value_2, result);
  }
  return flag;
}

int helper_add(int sign_value1, int sign_value2, s21_decimal *value_1,
               s21_decimal *value_2, s21_decimal *result) {
  int flag = 1;
  if ((sign_value1 == 1) && (sign_value2 == 0)) {
    change_places_value(value_1, value_2);
    s21_negate(*value_2, value_2);
    s21_negate(*result, result);
    flag = 0;
  }
  if ((sign_value1 == 0) && (sign_value2 == 1)) {
    s21_negate(*value_2, value_2);
    is_a_less_b_sub(value_1, value_2, result);
    flag = 0;
  }
  if ((sign_value1 == 1) && (sign_value2 == 1)) {
    s21_negate(*value_2, value_2);
    s21_negate(*value_1, value_1);
    s21_negate(*result, result);
  }
  return flag;
}

void is_a_less_b_sub(s21_decimal *value_1, s21_decimal *value_2,
                     s21_decimal *result) {
  if (s21_is_less(*value_1, *value_2)) {
    s21_decimal changer = *value_1;
    *value_1 = *value_2;
    *value_2 = changer;
    s21_negate(*result, result);
  }
}

void change_places_value(s21_decimal *value_1, s21_decimal *value_2) {
  s21_decimal changer = *value_1;
  *value_1 = *value_2;
  *value_2 = changer;
  s21_negate(*value_2, value_2);
  s21_negate(*value_1, value_1);
}

void check_overflow_add(int position, int bit, int bit_1, int bit_2,
                        int reminder, s21_decimal *result) {
  if ((position == 31) & (bit == 2)) {
    if (bit_1 + bit_2 + reminder >= 2) {
      result->value_type = DECIMAL_OVERFLOW;
    }
  }
}

void set_sign(s21_decimal *val, int sign) {
  sign ? set_sign_minus(val) : set_sign_plus(val);
}

int divide_by_10(s21_decimal *result) {
  unsigned long long u_num = result->bits[2];
  int last_digits = 0;
  for (int j = 2; j >= 0; j--) {
    last_digits = u_num % 10;
    result->bits[j] = u_num / 10;
    u_num = last_digits * (4294967296) + result->bits[j - 1];
  }
  result->bits[3] = 0;
  return last_digits;
}

void add_one(s21_decimal *value) {
  unsigned int plus_one = 1;
  for (int i = 0; i < 3 && plus_one; i++) {
    unsigned long long sum = (unsigned long long)value->bits[i] + plus_one;
    value->bits[i] = (unsigned int)sum;
    plus_one = (unsigned int)(sum >> 32);
  }
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int scale = getScale(value);
  int sign = getSign(value);

  while (scale > 0) {
    divide_by_10(&value);
    scale--;
  }

  *result = value;
  set_sign(result, sign);

  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int sign = getSign(value);
  s21_truncate(value, result);

  if (sign != 0) {
    add_one(result);
    set_sign(result, sign);
  }

  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int scale = getScale(value);
  int sign = getSign(value);
  int after_dot = 0;
  s21_decimal temp_value = value;

  for (; scale > 0; scale--) {
    after_dot = divide_by_10(&value);
  }
  int scale2 = getScale(temp_value);

  if (scale2 != 0) {
    s21_truncate(value, result);
    if (after_dot >= 5) add_one(&value);
  }

  *result = value;
  set_sign(result, sign);
  return 0;
}

int get_bit(s21_decimal value, int pos) {
  return (value.bits[pos / 32] >> (pos % 32)) & 1;
}

void set_sign_minus(s21_decimal *val) { val->bits[3] |= 1 << 31; }

void set_sign_plus(s21_decimal *val) { val->bits[3] &= ~(1 << 31); }

void s21_set_bit(s21_decimal *number, int bit, int value) {
  unsigned int shift = 1;
  if (bit >= 0 && bit < 128) {
    if (value)
      number->bits[bit / 32] = number->bits[bit / 32] | shift << (bit % 32);
    else
      number->bits[bit / 32] = number->bits[bit / 32] & ~(shift << (bit % 32));
  }
}
