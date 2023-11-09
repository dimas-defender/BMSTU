CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

app.exe : main.o input.o output.o func.o
	gcc -o $@ $^

unit_tests.exe : check_main.o check_insert.o check_copy.o check_sort.o func.o
	gcc -o $@ $^ -lcheck

check_insert.o : check_insert.c func.h
	gcc $(CFLAGS) -c $<
	
check_copy.o : check_copy.c func.h
	gcc $(CFLAGS) -c $<
	
check_sort.o : check_sort.c func.h
	gcc $(CFLAGS) -c $<

check_main.o : check_main.c suites.h
	gcc $(CFLAGS) -c $<

input.o : input.c input.h defines.h
	gcc $(CFLAGS) -c $<

output.o : output.c output.h defines.h
	gcc $(CFLAGS) -c $<

func.o : func.c func.h defines.h
	gcc $(CFLAGS) -c $<

main.o : main.c input.h output.h func.h defines.h
	gcc $(CFLAGS) -c $<

.PHONY : clean utest
clean :
	rm *.o *.exe

utest : unit_tests.exe
	./unit_tests.exe