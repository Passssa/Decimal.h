CC=gcc
CFLAGS=-Wall -Wextra -Werror
GCOVFLAGS=-fprofile-arcs -ftest-coverage
LEAK_FLAGS=-g -fsanitize=address
HEADER=s21_decimal.h
TESTS=project.out
SOURCES_ALL:=$(wildcard *.c)
SOURCES_FUN:=$(shell find . -not -name "s21_decimal_test.c" -name "*.c")
OBJECTS:=$(patsubst %.c,%.o,$(SOURCES_ALL))
OBJ_LIBRARY:=$(patsubst %.c,%.o,$(SOURCES_FUN))
UNAME:=$(shell uname -s)
ifeq ($(UNAME),Linux)
	LDFLAGS=-lcheck -lsubunit -lm -lrt -lpthread
	OPEN_CM=xdg-open
endif
ifeq ($(UNAME),Darwin)
	LDFLAGS=-lcheck
	OPEN_CM=open
endif

all : test s21_decimal.a gcov_report

check : s21_decimal_test.check
	checkmk clean_mode=1 s21_decimal_test.check > s21_decimal_test.c

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

test : check $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TESTS) $(LDFLAGS)
	./$(TESTS)

s21_decimal.a : $(OBJ_LIBRARY) $(HEADER)
	ar rcs s21_decimal.a $(OBJ_LIBRARY)
	ranlib s21_decimal.a

gcov_report : check
	$(CC) $(GCOVFLAGS) $(SOURCES_ALL) $(LDFLAGS) -o report.out
	./report.out
	gcov -f $(SOURCES_FUN)
	lcov -t "gcov_report" --no-external -o Coverage_Report.info -c -d .
	genhtml -o ./report Coverage_Report.info
	$(OPEN_CM) ./report/index-sort-f.html

rebuild : clean all


memory_test : check $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TESTS) $(LDFLAGS) $(LEAK_FLAGS)
	./$(TESTS)

clean :  
	rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -rf ./report
	rm -f CPPLINT.cfg
