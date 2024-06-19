FLAGS=-std=c11 -Wall -Werror -Wextra -g
SOURCES=s21_sprintf.c s21_sscanf.c s21_string.c
OBJECTS=$(SOURCES:.c=.o)
OS = $(shell uname)

ifeq ($(OS), Darwin)
	TEST_LIBS=-lcheck
else
	TEST_LIBS=-lcheck -lsubunit -pthread -lcheck_pic -lrt -lm
endif

all: s21_string.a test gcov_report

s21_string.a: clean $(OBJECTS)
	ar -rcs s21_string.a $(OBJECTS)
	rm -f *.o

test: s21_string_test.c s21_string.a
	gcc $(FLAGS) s21_string_test.c $(SOURCES) -o test $(TEST_LIBS) -L. --coverage
	./test

gcov_report: test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

install lcov:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

check: test
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h
	make test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt test.info test.dSYM

