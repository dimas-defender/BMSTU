CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

app.exe : main.o input.o output.o process.o
	gcc -o $@ $^

unit_tests.exe : check_main.o check_get_polynom_value.o check_get_derivative.o check_sum_polynom.o check_divide_into_even_and_odd.o process.o
	gcc -o $@ $^ -lcheck

check_get_polynom_value.o : check_get_polynom_value.c process.h
	gcc $(CFLAGS) -c $<
	
check_get_derivative.o : check_get_derivative.c process.h
	gcc $(CFLAGS) -c $<
	
check_sum_polynom.o : check_sum_polynom.c process.h
	gcc $(CFLAGS) -c $<
	
check_divide_into_even_and_odd.o : check_divide_into_even_and_odd.c process.h
	gcc $(CFLAGS) -c $<

check_main.o : check_main.c suites.h
	gcc $(CFLAGS) -c $<

input.o : input.c input.h defines.h
	gcc $(CFLAGS) -c $<

output.o : output.c output.h defines.h
	gcc $(CFLAGS) -c $<

process.o : process.c process.h defines.h
	gcc $(CFLAGS) -c $<

main.o : main.c input.h output.h process.h defines.h
	gcc $(CFLAGS) -c $<

.PHONY : clean utest ftest
clean :
	rm *.o *.exe

utest : unit_tests.exe
	./unit_tests.exe
	
ftest : app.exe
	./run_tests.bat