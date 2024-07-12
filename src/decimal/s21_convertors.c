#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 0;
  if (src != S21_INF && src != S21_NAN) {
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0;
    if (src < 0) {
      if (src == -2147483648) {
        src += 1;
      }
      dst->bits[3] = (1 << 31);
      src *= -1;
    }
    dst->bits[0] = src;
  } else
    res = 1;
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 0;
  int sign = (src.bits[3] >> 31);
  int scale = (src.bits[3] << 1) >> 17;
  int temp = 0;
  if (scale) {
    s21_decimal trunc = {{0, 0, 0, 0}, 0};
    s21_truncate(src, &trunc);
    if (src.bits[1] || src.bits[2] || ((src.bits[0] >> 31))) {
      res = 1;
    }
    temp = trunc.bits[0];
  } else {
    if (src.bits[1] || src.bits[2] || ((src.bits[0] >> 31))) {
      res = 1;
    }
    temp = src.bits[0];
  }
  *dst = temp;
  if (sign) {
    *dst *= (-1);
  }

  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 0;
  int flag = 0;
  if (src != S21_INF && src != S21_NAN) {
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0;
    int scale = 0;
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src *= -1;
    }
    if (src) {
      unsigned int fbits = *((unsigned int *)&src);
      int float_exp = 0;
      float_exp = (((fbits) << 1) >> 24) - 127;
      float after_tochka = 0;
      double before_tochka = 0;
      after_tochka = modf(src, &before_tochka);
      if (float_exp > 95) res = 1;
      if (float_exp <= 95) {
        float less_one_check = src;
        for (; !((int)src); src *= 10, scale++) {
        }
        if (!flag && (less_one_check != src)) {
          src *= 1000000;
          scale += 6;
        } else {
          for (; after_tochka && !(int)(src / 1000000); src *= 10, scale++) {
          }
          src = roundf(src);
        }
        while (after_tochka && !((int)src % 10)) {
          src = src / 10.0;
          scale--;
        }
        unsigned int fbits = *((unsigned int *)&src);
        float_exp = (((fbits) << 1) >> 24) - 127;
        if (float_exp < -94 || scale > 28) res = 1;

        s21_set_bit(dst, float_exp, 1);
        for (int i = 22; i >= 0; i--) {
          float_exp--;
          if ((fbits & 1 << i) >> i) {
            s21_set_bit(dst, float_exp, 1);
          }
        }
        if (sign) {
          dst->bits[3] |= 1 << 31;
        }
        if (scale) {
          dst->bits[3] |= scale << 16;
        }
      }
    }
  } else
    res = 1;
  if (res) {
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0;
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double dub_temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    dub_temp += get_bit(src, i) * pow(2, i);
  }
  dub_temp = dub_temp / pow(10, (src.bits[3] << 1) >> 17);
  if (get_bit(src, 127)) dub_temp *= -1;
  *dst = dub_temp;
  return 0;
}
