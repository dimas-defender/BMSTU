CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

app.exe : main.o input.o output.o process.o
	gcc -o $@ $^
	
unit_tests.exe : check_main.o check_make_matrix_square.o check_expand_square_matrix.o check_calculate_matrix_expression.o process.o
	gcc -o $@ $^ -lcheck

check_make_matrix_square.o : check_make_matrix_square.c process.h
	gcc $(CFLAGS) -c $<
	
check_expand_square_matrix.o : check_expand_square_matrix.c process.h defines.h
	gcc $(CFLAGS) -c $<

check_calculate_matrix_expression.o : check_calculate_matrix_expression.c process.h defines.h
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