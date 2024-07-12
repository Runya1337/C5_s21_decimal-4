#include "s21_decimal.h"

#include <check.h>
#include <time.h>
#include <unistd.h>

START_TEST(test_dec_to_int_1) {
  s21_decimal test_dec = {{20, 0, 0, 0}, 0};
  int param = 0, need_param = 20, error = 0;
  int func_error = s21_from_decimal_to_int(test_dec, &param);
  ck_assert_int_eq(param, need_param);
  ck_assert_int_eq(error, func_error);
}
END_TEST

START_TEST(test_dec_to_int_2) {
  s21_decimal test_dec = {{20, 0, 0, 2147483648}, 0};
  int param;
  s21_from_decimal_to_int(test_dec, &param);
  ck_assert_int_eq(param, -20);
}
END_TEST

START_TEST(test_dec_to_int_3) {
  s21_decimal test_dec = {{2147483648, 0, 0, 2147483648}, 0};
  int param, need_param = -2147483648;
  s21_from_decimal_to_int(test_dec, &param);
  ck_assert_int_eq(param, need_param);
}
END_TEST

START_TEST(test_dec_to_int_4) {
  s21_decimal test_dec = {{2147483647, 0, 1, 2147483648}, 0};
  int param;
  int error = s21_from_decimal_to_int(test_dec, &param);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_dec_to_int_5) {
  s21_decimal test_dec = {{2147483647, 1, 0, 2147483648}, 0};
  int param;
  int error = s21_from_decimal_to_int(test_dec, &param);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_dec_to_int_6) {
  s21_decimal test_dec = {{2147483647, 1, 0, 0x30000}, 0};  // exp = 3
  int param;
  s21_from_decimal_to_int(test_dec, &param);
  ck_assert_int_eq(param, 6442450);
}
END_TEST

/*
*** TEST dec_to_float
*/

START_TEST(test_dec_to_float_1) {
  s21_decimal test_dec = {{5, 0, 0, 2147483648}, 0};
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, -5.0);
}
END_TEST

START_TEST(test_dec_to_float_2) {
  s21_decimal test_dec = {{5, 0, 0, 0}, 0};
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, 5.0);
}
END_TEST

START_TEST(test_dec_to_float_3) {
  s21_decimal test_dec = {{5, 0, 0, 131072}, 0};
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, 0.05);
}
END_TEST

START_TEST(test_dec_to_float_4) {
  s21_decimal test_dec = {{5, 0, 0, 262144}, 0};
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, 0.0005);
}
END_TEST

START_TEST(test_dec_to_float_5) {
  s21_decimal test_dec = {{5, 0, 0, 2147549184}, 0};
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, -0.5);
}
END_TEST

START_TEST(test_dec_to_float_6) {
  s21_decimal test_dec = {{5, 0, 0, 2147614720}, 0};
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, -0.05);
}
END_TEST

START_TEST(test_dec_to_float_7) {
  s21_decimal test_dec = {{5, 1, 0, 2147614720}, 0};  // -4294967301 * 10^-2
  float param = 0;

  s21_from_decimal_to_float(test_dec, &param);
  ck_assert_float_eq(param, -42949672);
}
END_TEST

/*
*** TEST int_to_dec
*/

START_TEST(test_int_to_dec_1) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  int param = 5;

  s21_from_int_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 5);
  ck_assert_float_eq(test_dec.bits[3], 0);
}
END_TEST

START_TEST(test_int_to_dec_2) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  int param = 2147483647;

  s21_from_int_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 2147483647);
  ck_assert_float_eq(test_dec.bits[3], 0);
}
END_TEST

START_TEST(test_int_to_dec_3) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  int param = -2147483648;

  s21_from_int_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 2147483648);
  ck_assert_float_eq(test_dec.bits[3], 2147483648);
}
END_TEST

START_TEST(test_int_to_dec_4) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  int param = -5;

  s21_from_int_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 5);
  ck_assert_float_eq(test_dec.bits[3], 2147483648);
}
END_TEST

/*
*** TEST float_to_dec
*/

START_TEST(test_float_to_dec_1) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = 5.0, need_param = 5.0;
  int need_3_bit = 0;
  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], need_param);
  ck_assert_float_eq(test_dec.bits[3], need_3_bit);
}
END_TEST

START_TEST(test_float_to_dec_2) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = 0.05;

  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 5);
  ck_assert_float_eq(test_dec.bits[3], 131072);
}
END_TEST

START_TEST(test_float_to_dec_4) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = -0.5;

  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 5);
  ck_assert_float_eq(test_dec.bits[3], 2147549184);
}
END_TEST

START_TEST(test_float_to_dec_5) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = -0.05;

  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 5);
  ck_assert_float_eq(test_dec.bits[3], 2147614720);
}
END_TEST

START_TEST(test_float_to_dec_6) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = 50;
  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 50);
  ck_assert_float_eq(test_dec.bits[3], 0);
}
END_TEST

START_TEST(test_float_to_dec_7) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = 5000;
  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 5000);
  ck_assert_float_eq(test_dec.bits[3], 0);
}
END_TEST

START_TEST(test_float_to_dec_8) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = 2147483;
  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 2147483);
  ck_assert_float_eq(test_dec.bits[3], 0);
}
END_TEST

START_TEST(test_float_to_dec_11) {
  s21_decimal test_dec = {{0, 0, 0, 0}, 0};
  float param = -2147483;
  s21_from_float_to_decimal(param, &test_dec);
  ck_assert_float_eq(test_dec.bits[0], 2147483);
  ck_assert_float_eq(test_dec.bits[3], 2147483648);
}
END_TEST

START_TEST(s21_negate_1) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000010100101;  // -16.5
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000010100101;  // 16.5
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal res1;
  s21_negate(dec1, &res1);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_negate_2) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000010100101;  // 16.5
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000010100101;  // -16.5
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal res1;
  s21_negate(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

/*  s21_truncate  */

START_TEST(s21_truncate_1) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000010100101;  // 16.5
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000010000;  // 16
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate_2) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000011010011;  // 21.1
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000010101;  // 21
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_int_eq(res1.bits[0], result.bits[0]);
  ck_assert_int_eq(res1.bits[1], result.bits[1]);
  ck_assert_int_eq(res1.bits[2], result.bits[2]);
  ck_assert_int_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate_3) {
  s21_decimal dec1;
  dec1.bits[0] =
      0b11111111111111111111111111111111;  // 7922816251426433759354395033.5
  dec1.bits[1] = 0b11111111111111111111111111111111;
  dec1.bits[2] = 0b11111111111111111111111111111111;
  dec1.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] =
      0b10011001100110011001100110011001;  // 7922816251426433759354395033
  result.bits[1] = 0b10011001100110011001100110011001;
  result.bits[2] = 0b00011001100110011001100110011001;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_int_eq(res1.bits[0], result.bits[0]);
  ck_assert_int_eq(res1.bits[1], result.bits[1]);
  ck_assert_int_eq(res1.bits[2], result.bits[2]);
  ck_assert_int_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate_5) {
  s21_decimal dec1;
  dec1.bits[0] = 0b11011110101001001000000010110110;  // 9.876543212346E+27
  dec1.bits[1] = 0b01011111101001011101111110100000;
  dec1.bits[2] = 0b00011111111010011010111101100000;
  dec1.bits[3] = 0b00000000000010010000000000000000;  // 9 степень
  s21_decimal result;
  result.bits[0] = 0b00110100101011011100000100111110;  // 9.876543212346E+18
  result.bits[1] = 0b10001001000100001000011110111001;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_int_eq(res1.bits[0], result.bits[0]);
  ck_assert_int_eq(res1.bits[1], result.bits[1]);
  ck_assert_int_eq(res1.bits[2], result.bits[2]);
  ck_assert_int_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate_6) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000011010011;  // 21.1
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000011010011;  // 21.1
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_int_eq(res1.bits[0], result.bits[0]);
  ck_assert_int_eq(res1.bits[1], result.bits[1]);
  ck_assert_int_eq(res1.bits[2], result.bits[2]);
  ck_assert_int_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate_7) {
  s21_decimal dec1;
  dec1.bits[0] =
      0b11100110001001011001001101101001;  //-579895323215489956.67897455465;

  dec1.bits[1] = 0b00111000110110101110001010110100;
  dec1.bits[2] = 0b10111011010111111101000100011110;
  dec1.bits[3] = 0b10000000000010110000000000000000;

  s21_decimal result;
  result.bits[0] = 0b01011011001010110110101110100100;  // 579895323215489956
  result.bits[1] = 0b00001000000011000011001110111001;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;

  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_int_eq(res1.bits[0], result.bits[0]);
  ck_assert_int_eq(res1.bits[1], result.bits[1]);
  ck_assert_int_eq(res1.bits[2], result.bits[2]);
  ck_assert_int_eq(res1.bits[3], result.bits[3]);
}
END_TEST

/*  s21_floor  */

START_TEST(s21_floor_1) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111100000001110001010010;  // 123,9432567890
  dec1.bits[1] = 0b00000000000000000000000100100000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000010100000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000001111011;  // 123
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_2) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000010100101;
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000010000;
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_3) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111111111111110001010010;  // 2483027.026
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000110000000000000000;  // 3
  s21_decimal result;
  result.bits[0] = 0b00000000001001011110001101010011;  // 2483027
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_4) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111111111111110001010011;
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000110000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000001001011110001101010100;
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_5) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00001111111111111111111111111111;
  dec1.bits[1] = 0b00111110001001010000001001100001;
  dec1.bits[2] = 0b00100000010011111100111001011110;
  dec1.bits[3] = 0b00000000000010100000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  s21_decimal result;
  result.bits[0] = 0b10100111011000111111111111111111;
  result.bits[1] = 0b00001101111000001011011010110011;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_6) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00001111111111111111111111111111;  //-1000000000.0000000000
  dec1.bits[1] = 0b00111110001001010000001001100001;
  dec1.bits[2] = 0b00100000010011111100111001011110;
  dec1.bits[3] = 0b10000000000010100000000000000000;  // 10
  s21_decimal res1;
  s21_floor(dec1, &res1);
  s21_decimal result;
  result.bits[0] = 0b10100111011001000000000000000000;  //-1000000000000000000
  result.bits[1] = 0b00001101111000001011011010110011;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_7) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10111101001001111000010001010010;  //-123.432567890
  dec1.bits[1] = 0b00000000000000000000000000011100;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000010010000000000000000;  // 9
  s21_decimal res1;
  s21_floor(dec1, &res1);
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000001111100;  //-124
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_8) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111111111111110001010011;  //-2483027.027
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000110000000000000000;  // 3
  s21_decimal result;
  result.bits[0] = 0b00000000001001011110001101010100;  //-2483028
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_9) {
  s21_decimal dec1;
  dec1.bits[0] = 0b01010101001110101110101110110001;  //-0.987654321287654321
  dec1.bits[1] = 0b00001101101101001101101001011111;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000100100000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000000001;  //-1
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor_10) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000000100011;  //-3,5
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000010000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000000100;  //-4
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_1) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111100000001110001010010;
  dec1.bits[1] = 0b00000000000000000000000100100000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000010100000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000001111100;
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_2) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000010100101;  // -16.5
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000010000000000000000;  // 1
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000010001;  //-17
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_3) {
  s21_decimal dec1;
  dec1.bits[0] = 0b11111111111111111111111111111111;  // 18014398509481.983
  dec1.bits[1] = 0b00000000001111111111111111111111;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000110000000000000000;
  s21_decimal result;
  result.bits[0] = 0b01001101110100101111000110101001;  // 18014398509481
  result.bits[1] = 0b00000000000000000001000001100010;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_4) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000011110111001001110100;  // -1012.340
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000000110000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000001111110100;  // -1012
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_5) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10000000000000000000001111110100;  // -112590205.4327284
  dec1.bits[1] = 0b00000000000001000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000001110000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000110101101011111110101111101;  // -112590205
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_6) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10000000000000000000001111110100;  // 2147484660
  dec1.bits[1] = 0b00000000000000000000000000000000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal result;
  result.bits[0] = 0b10000000000000000000001111110100;  // 2147484660
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_7) {
  s21_decimal dec1;
  dec1.bits[0] = 0b01100011000011111111111111111111;  // -10000000000.0000000000
  dec1.bits[1] = 0b01101011110001110101111000101101;
  dec1.bits[2] = 0b00000000000000000000000000000101;
  dec1.bits[3] = 0b10000000000010100000000000000000;  // 10
  s21_decimal result;
  result.bits[0] = 0b01010100000010111110010000000000;  // -10000000000
  result.bits[1] = 0b00000000000000000000000000000010;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_8) {
  s21_decimal dec1;
  dec1.bits[0] = 0b01010101001110101110101110110001;  //-0.987654321287654321
  dec1.bits[1] = 0b00001101101101001101101001011111;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b10000000000100100000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000000000001;  // -1
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_9) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111100000001110001010010;
  dec1.bits[1] = 0b00000000000000000000000100100000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000010100000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000001111100;
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_round_10) {
  s21_decimal dec1;
  dec1.bits[0] = 0b11001101110001110111110001000000;
  dec1.bits[1] = 0b00111110001001010000001001100000;
  dec1.bits[2] = 0b00100000010011111100111001011110;
  dec1.bits[3] = 0b10000000000001000000000000000000;
  s21_decimal result;
  result.bits[0] = 0b10100000111111100100111000000100;
  result.bits[1] = 0b00011011110011101100110011101101;
  result.bits[2] = 0b00000000000000001101001111000010;
  result.bits[3] = 0b10000000000000000000000000000000;
  s21_decimal res1;
  s21_round(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(test_s21_is_less_1) {
  s21_decimal test_v_1 = {{10, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{15, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_less_2) {
  s21_decimal test_v_1 = {{6739, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{19993, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_less_3) {
  s21_decimal test_v_1 = {{19993, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{6739, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_4) {
  s21_decimal test_v_1 = {{19993, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{6739, 0, 0, 2147483648}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_5) {
  s21_decimal test_v_1 = {{19993, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{19993, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_6) {
  s21_decimal test_v_1 = {{19993, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{19993, 0, 0, 131072}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_7) {
  s21_decimal test_v_1 = {{0, 1, 0, 0}, 0};
  s21_decimal test_v_2 = {{19993, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_8) {
  s21_decimal test_v_1 = {{19993, 1, 0, 2147483648}, 0};
  s21_decimal test_v_2 = {{19993, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_less_9) {
  s21_decimal value_1 = {{5000, 5000, 5000, 0x10000}, 0};
  s21_decimal value_2 = {{5000, 0, 0, 0x20000}, 0};

  ck_assert_int_eq(s21_is_less(value_1, value_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_10) {
  s21_decimal value_1 = {{5000, 0, 0, 0x80000000}, 0};
  s21_decimal value_2 = {{5000, 0, 0, 0x80000000}, 0};

  ck_assert_int_eq(s21_is_less(value_1, value_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_1) {
  s21_decimal test_v_1 = {{0, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{0, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_2) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5001, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_3) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 2147483648}, 0};
  ck_assert_int_eq(s21_is_less_or_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_4) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 131072}, 0};
  ck_assert_int_eq(s21_is_less_or_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_5) {
  s21_decimal test_v_1 = {{0, 1, 0, 0}, 0};
  s21_decimal test_v_2 = {{0, 1, 0, 0}, 0};
  ck_assert_int_eq(s21_is_less_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_greater_1) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5001, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_greater(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_greater_2) {
  s21_decimal test_v_1 = {{5001, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_greater(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_greater_3) {
  s21_decimal test_v_1 = {{5168465, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5001, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_greater(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_greater_4) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0x40000}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 0x20000}, 0};
  ck_assert_int_eq(s21_is_greater(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_greater_5) {
  s21_decimal value_1 = {{5000, 0, 0, 2}, 0};
  s21_decimal value_2 = {{5000, 0, 0, 3}, 0};

  ck_assert_int_eq(s21_is_greater(value_1, value_2), 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_1) {
  s21_decimal test_v_1 = {{0, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{0, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_greater_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_2) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5001, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_greater_or_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_3) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 131072}, 0};
  ck_assert_int_eq(s21_is_greater_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_4) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 2147483648}, 0};
  ck_assert_int_eq(s21_is_greater_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_5) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_greater_or_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_equal_1) {
  s21_decimal test_v_1 = {{0, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{0, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_equal_2) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5001, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_equal_3) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 2147483648}, 0};
  ck_assert_int_eq(s21_is_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_equal_4) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 131072}, 0};
  ck_assert_int_eq(s21_is_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_equal_5) {
  s21_decimal test_v_1 = {{5000, 1, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 2, 0}, 0};
  ck_assert_int_eq(s21_is_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_not_equal_1) {
  s21_decimal test_v_1 = {{0, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{0, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_not_equal(test_v_1, test_v_2), 0);
}
END_TEST

START_TEST(test_s21_is_not_equal_2) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5001, 0, 0, 0}, 0};
  ck_assert_int_eq(s21_is_not_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_3) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 2147483648}, 0};
  ck_assert_int_eq(s21_is_not_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_4) {
  s21_decimal test_v_1 = {{5000, 0, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 0, 131072}, 0};
  ck_assert_int_eq(s21_is_not_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_5) {
  s21_decimal test_v_1 = {{5000, 1, 0, 0}, 0};
  s21_decimal test_v_2 = {{5000, 0, 2, 0}, 0};
  ck_assert_int_eq(s21_is_not_equal(test_v_1, test_v_2), 1);
}
END_TEST

START_TEST(add_a_positive_notfloat_small_b_positive_notfloat_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {{0b00000000000000000000000000001100, 0, 0,
                           0b00000000000000000000000000000000},
                          0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_negative_notfloat_small_b_negative_notfloat_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {{0b00000000000000000000000000001100, 0, 0,
                           0b10000000000000000000000000000000},
                          0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_positive_notfloat_small_b_negative_notfloat_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {{0b00000000000000000000000000000010, 0, 0,
                           0b00000000000000000000000000000000},
                          0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_negative_notfloat_small_b_positive_notfloat_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {{0b00000000000000000000000000000010, 0, 0,
                           0b10000000000000000000000000000000},
                          0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_positive_float_small_b_positive_float_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  setScale(&a, 4);
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  setScale(&b, 5);
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_add(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000000000001001011, 0, 0,
                           0b00000000000000000000000000000000},
                          0};
  setScale(&c_answer, 5);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_negative_float_small_b_positive_float_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b10000000000000110000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b00000000000001000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {{0b00000000000000000000000001000001, 0, 0,
                           0b10000000000001000000000000000000},
                          0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_positive_notfloat_medium_b_positive_notfloat_medium) {
  s21_decimal a = {
      {0b00000000000000000000000000000111, 0b00000000000000000000000000000111,
       0, 0b00000000000000000000000000000000},
      0};
  s21_decimal b = {
      {0b00000000000000000000000000000101, 0b00000000000000000000000000000101,
       0, 0b00000000000000000000000000000000},
      0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {
      {0b00000000000000000000000000001100, 0b00000000000000000000000000001100,
       0, 0b00000000000000000000000000000000},
      0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_negative_notfloat_medium_b_positive_notfloat_medium) {
  s21_decimal a = {
      {0b00000000000000000000000000000111, 0b00000000000000000000000000000111,
       0, 0b10000000000000000000000000000000},
      0};
  s21_decimal b = {
      {0b00000000000000000000000000000101, 0b00000000000000000000000000000101,
       0, 0b00000000000000000000000000000000},
      0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {
      {0b00000000000000000000000000000010, 0b00000000000000000000000000000010,
       0, 0b10000000000000000000000000000000},
      0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_positive_float_medium_b_positive_float_medium) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  setScale(&a, 5);
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  setScale(&b, 25);
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_add(a, b, &c);
  s21_decimal c_answer = {
      {0b10110101011100000000000000000101, 0b11110010011100111001001100111101,
       0b00000000000000000000000000100101, 0b00000000000000000000000000000000},
      0};
  setScale(&c_answer, 25);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_negative_float_medium_b_positive_float_medium) {
  s21_decimal a = {
      {0b00000000000000000000000000000111, 0b00000000000000000000000000000111,
       0, 0b10000000000011110000000000000000},
      0};
  setScale(&a, 5);
  s21_decimal b = {
      {0b00000000000000000000000000000101, 0b00000000000000000000000000000101,
       0, 0b00000000000011100000000000000000},
      0};
  setScale(&b, 3);
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {
      {0b00000000000000000000000000000010, 0b00000000000000000000000000000010,
       0, 0b10000000000011110000000000000000},
      0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_positive_notfloat_large_b_positive_notfloat_large) {
  s21_decimal a = {
      {0b00000000000000000000000000000111, 0b00000000000000000000000000000111,
       0b00000000000000000000000000000111, 0b00000000000000000000000000000000},
      0};
  s21_decimal b = {
      {0b00000000000000000000000000000101, 0b00000000000000000000000000000101,
       0b00000000000000000000000000000101, 0b00000000000000000000000000000000},
      0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_add(a, b, &c);
  s21_decimal c_answer = {
      {0b00000000000000000000000000001100, 0b00000000000000000000000000001100,
       0b00000000000000000000000000001100, 0},
      0};
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_negative_notfloat_large_b_positive_notfloat_large) {
  s21_decimal a = {
      {0b00000000000000000000000000000111, 0b00000000000000000000000000000111,
       0b00000000000000000000000000000111, 0b10000000000000000000000000000000},
      0};
  s21_decimal b = {
      {0b00000000000000000000000000000101, 0b00000000000000000000000000000101,
       0b00000000000000000000000000000101, 0b00000000000000000000000000000000},
      0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {
      {0b00000000000000000000000000000010, 0b00000000000000000000000000000010,
       0b00000000000000000000000000000010, 0b10000000000000000000000000000000},
      0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(add_a_positive_float_large_b_positive_float_large) {
  s21_decimal a = {
      {0b00000000000000000000000000000111, 0b00000000000000000000000000000111,
       0b00000000000000000000000000000111, 0b00000000000110010000000000000000},
      0};
  s21_decimal b = {
      {0b00000000000000000000000000000101, 0b00000000000000000000000000000101,
       0b00000000000000000000000000000101, 0b00000000000110000000000000000000},
      0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_decimal c_answer = {
      {0b00000000000000000000000000111001, 0b00000000000000000000000000111001,
       0b00000000000000000000000000111001, 0b00000000000110010000000000000000},
      0};
  s21_add(a, b, &c);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(mul_two_small_positive_floats) {
  s21_decimal a = {{0b00000000000000000000000000000010, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  setScale(&a, 2);
  s21_decimal b = {{0b00000000000000000000000000000011, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  setScale(&b, 2);
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_mul(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000000000000000110, 0, 0,
                           0b00000000000000000000000000000000},
                          0};
  setScale(&c_answer, 4);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(mul_two_positive_integers) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0, 0}, 0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0, 0}, 0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_mul(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000000000000100011, 0, 0, 0}, 0};
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(sub_a_small_positive_float_from_large_positive_float) {
  s21_decimal a = {{0b00000000000000000000001111101101, 0, 0, 0}, 0};
  setScale(&a, 1);
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0, 0}, 0};
  setScale(&b, 2);
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_sub(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000010011100111101, 0, 0, 0}, 0};
  setScale(&c_answer, 2);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(sub_a_large_negative_float_from_small_negative_float) {
  s21_decimal a = {{0b00000000000000000000000000000101, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  setScale(&a, 1);
  s21_decimal b = {{0b00000000000000000010011100010101, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  setScale(&b, 2);
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_sub(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000010011011100011, 0, 0, 0}, 0};
  setScale(&c_answer, 2);
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(sub_a_positive_integer_from_negative_integer) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0, 0}, 0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_sub(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000000000000001100, 0, 0,
                           0b10000000000000000000000000000000},
                          0};
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(sub_a_negative_integer_from_positive_integer) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0, 0}, 0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b10000000000000000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  s21_sub(a, b, &c);
  s21_decimal c_answer = {{0b00000000000000000000000000001100, 0, 0, 0}, 0};
  int equel_answer = s21_is_equal(c, c_answer);
  ck_assert_int_eq(equel_answer, 1);
}
END_TEST

START_TEST(div_a_positive_float_small_b_positive_float_small) {
  s21_decimal a = {{0b00000000000000000000000000000111, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  s21_decimal b = {{0b00000000000000000000000000000101, 0, 0,
                    0b00000000000000000000000000000000},
                   0};
  s21_decimal c = {{0, 0, 0, 0}, 0};
  int equel_answer = s21_div(a, b, &c);
  ck_assert_int_eq(equel_answer, 0);
}
END_TEST

Suite* my_tests_suite() {
  Suite* suite;
  TCase* tc;

  suite = suite_create("TESTS");

  tc = tcase_create("TESTS");

  suite_add_tcase(suite, tc);

  tcase_add_test(tc, test_dec_to_int_1);
  tcase_add_test(tc, test_dec_to_int_3);
  tcase_add_test(tc, test_dec_to_int_4);
  tcase_add_test(tc, test_dec_to_int_5);
  tcase_add_test(tc, test_dec_to_int_6);
  tcase_add_test(tc, test_int_to_dec_1);
  tcase_add_test(tc, test_int_to_dec_2);
  tcase_add_test(tc, test_int_to_dec_3);
  tcase_add_test(tc, test_int_to_dec_4);
  tcase_add_test(tc, test_dec_to_int_2);
  tcase_add_test(tc, test_dec_to_float_1);
  tcase_add_test(tc, test_dec_to_float_2);
  tcase_add_test(tc, test_dec_to_float_3);
  tcase_add_test(tc, test_dec_to_float_5);
  tcase_add_test(tc, test_dec_to_float_6);
  tcase_add_test(tc, test_dec_to_float_7);
  tcase_add_test(tc, test_dec_to_float_4);
  tcase_add_test(tc, test_float_to_dec_1);
  tcase_add_test(tc, test_float_to_dec_4);
  tcase_add_test(tc, test_float_to_dec_2);
  tcase_add_test(tc, test_float_to_dec_5);
  tcase_add_test(tc, test_float_to_dec_6);
  tcase_add_test(tc, test_float_to_dec_7);
  tcase_add_test(tc, test_float_to_dec_8);
  tcase_add_test(tc, test_float_to_dec_11);

  tcase_add_test(tc, s21_negate_1);
  tcase_add_test(tc, s21_negate_2);

  tcase_add_test(tc, s21_truncate_1);
  tcase_add_test(tc, s21_truncate_2);
  tcase_add_test(tc, s21_truncate_3);
  tcase_add_test(tc, s21_truncate_5);
  tcase_add_test(tc, s21_truncate_6);
  tcase_add_test(tc, s21_truncate_7);

  tcase_add_test(tc, s21_floor_1);
  tcase_add_test(tc, s21_floor_2);
  tcase_add_test(tc, s21_floor_3);
  tcase_add_test(tc, s21_floor_4);
  tcase_add_test(tc, s21_floor_5);
  tcase_add_test(tc, s21_floor_6);
  tcase_add_test(tc, s21_floor_7);
  tcase_add_test(tc, s21_floor_8);
  tcase_add_test(tc, s21_floor_9);
  tcase_add_test(tc, s21_floor_10);

  tcase_add_test(tc, s21_round_1);
  tcase_add_test(tc, s21_round_2);
  tcase_add_test(tc, s21_round_3);
  tcase_add_test(tc, s21_round_4);
  tcase_add_test(tc, s21_round_5);
  tcase_add_test(tc, s21_round_6);
  tcase_add_test(tc, s21_round_7);
  tcase_add_test(tc, s21_round_8);
  tcase_add_test(tc, s21_round_9);
  tcase_add_test(tc, s21_round_10);

  tcase_add_test(tc, test_s21_is_less_1);
  tcase_add_test(tc, test_s21_is_less_2);
  tcase_add_test(tc, test_s21_is_less_3);
  tcase_add_test(tc, test_s21_is_less_4);
  tcase_add_test(tc, test_s21_is_less_5);
  tcase_add_test(tc, test_s21_is_less_6);
  tcase_add_test(tc, test_s21_is_less_7);
  tcase_add_test(tc, test_s21_is_less_8);
  tcase_add_test(tc, test_s21_is_less_9);
  tcase_add_test(tc, test_s21_is_less_10);

  tcase_add_test(tc, test_s21_is_less_or_equal_1);
  tcase_add_test(tc, test_s21_is_less_or_equal_2);
  tcase_add_test(tc, test_s21_is_less_or_equal_3);
  tcase_add_test(tc, test_s21_is_less_or_equal_4);
  tcase_add_test(tc, test_s21_is_less_or_equal_5);

  tcase_add_test(tc, test_s21_is_greater_1);
  tcase_add_test(tc, test_s21_is_greater_2);
  tcase_add_test(tc, test_s21_is_greater_3);
  tcase_add_test(tc, test_s21_is_greater_4);
  tcase_add_test(tc, test_s21_is_greater_5);

  tcase_add_test(tc, test_s21_is_greater_or_equal_1);
  tcase_add_test(tc, test_s21_is_greater_or_equal_2);
  tcase_add_test(tc, test_s21_is_greater_or_equal_3);
  tcase_add_test(tc, test_s21_is_greater_or_equal_4);
  tcase_add_test(tc, test_s21_is_greater_or_equal_5);

  tcase_add_test(tc, test_s21_is_equal_1);
  tcase_add_test(tc, test_s21_is_equal_2);
  tcase_add_test(tc, test_s21_is_equal_3);
  tcase_add_test(tc, test_s21_is_equal_4);
  tcase_add_test(tc, test_s21_is_equal_5);

  tcase_add_test(tc, test_s21_is_not_equal_1);
  tcase_add_test(tc, test_s21_is_not_equal_2);
  tcase_add_test(tc, test_s21_is_not_equal_3);
  tcase_add_test(tc, test_s21_is_not_equal_4);
  tcase_add_test(tc, test_s21_is_not_equal_5);

  tcase_add_test(tc, add_a_positive_notfloat_small_b_positive_notfloat_small);
  tcase_add_test(tc, add_a_negative_notfloat_small_b_negative_notfloat_small);
  tcase_add_test(tc, add_a_positive_notfloat_small_b_negative_notfloat_small);
  tcase_add_test(tc, add_a_negative_notfloat_small_b_positive_notfloat_small);
  tcase_add_test(tc, add_a_negative_float_small_b_positive_float_small);
  tcase_add_test(tc, add_a_positive_float_small_b_positive_float_small);
  tcase_add_test(tc, add_a_positive_notfloat_medium_b_positive_notfloat_medium);
  tcase_add_test(tc, add_a_negative_notfloat_medium_b_positive_notfloat_medium);
  tcase_add_test(tc, add_a_positive_float_medium_b_positive_float_medium);
  tcase_add_test(tc, add_a_negative_float_medium_b_positive_float_medium);
  tcase_add_test(tc, add_a_positive_notfloat_large_b_positive_notfloat_large);
  tcase_add_test(tc, add_a_negative_notfloat_large_b_positive_notfloat_large);
  tcase_add_test(tc, add_a_positive_float_large_b_positive_float_large);

  tcase_add_test(tc, mul_two_small_positive_floats);
  tcase_add_test(tc, mul_two_positive_integers);

  tcase_add_test(tc, sub_a_small_positive_float_from_large_positive_float);
  tcase_add_test(tc, sub_a_large_negative_float_from_small_negative_float);
  tcase_add_test(tc, sub_a_positive_integer_from_negative_integer);
  tcase_add_test(tc, sub_a_negative_integer_from_positive_integer);

  tcase_add_test(tc, div_a_positive_float_small_b_positive_float_small);

  suite_add_tcase(suite, tc);

  return suite;
}
int main() {
  int number_failed = 0;
  Suite* suite = my_tests_suite();
  SRunner* srunner = srunner_create(suite);

  // suite = my_tests_suite();
  // srunner = srunner_create(suite);

  srunner_run_all(srunner, CK_NORMAL);
  number_failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
