CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

unit_tests.exe : check_main.o check_snprintf.o my_snprintf.o
	gcc -o $@ $^ -lcheck

check_snprintf.o : check_snprintf.c my_snprintf.h
	gcc $(CFLAGS) -c $<

check_main.o : check_main.c
	gcc $(CFLAGS) -c $<

my_snprintf.o : my_snprintf.c my_snprintf.h
	gcc $(CFLAGS) -c $<

.PHONY : clean utest
clean :
	rm *.o *.exe

utest : unit_tests.exe
	./unit_tests.exe