CC = gcc
TESTFLAGS = -lcheck -g
CFLAGS = -Wall -Wextra -Werror -std=c11
TESTFLAGS2 = -lcheck -lm -lpthread -lrt -lsubunit -g
GCOVFLAGS = -fprofile-arcs -ftest-coverage

all: test

s21_decimal.a : decimal/*.c
	$(CC) $(CFLAGS) -g -std=c11 -c decimal/*.c
	ar rcs s21_decimal.a *.o
	rm -rf *.o

test: s21_decimal_test.c decimal/*.c
	$(CC) $(CFLAGS) s21_decimal_test.c decimal/*.c -o test $(TESTFLAGS)

gcov_report:
	$(CC) $(GCOVFLAGS) $(CFLAGS) s21_decimal_test.c decimal/*.c -o test $(TESTFLAGS)
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	rm -rf gcovreport gcovreport.info *.gcda *.gcno string_test.gcda string_test.gcno
	open report/index.html
rebuild: clean all

clean: 
	rm -rf *.o test *.a a.out *.gcno *.gcda test.dSYM *.info report

style_check:
	clang-format -style=Google -n *.c *.h *.c

style:
	clang-format -style=Google -i *.c *.h *.c
