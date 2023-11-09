CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

app.exe : main.o input.o output.o process.o
	gcc -o $@ $^

unit_tests.exe : check_main.o check_sort_by_density.o check_filter_items.o process.o
	gcc -o $@ $^ -lcheck

check_sort_by_density.o : check_sort_by_density.c process.h defines.h
	gcc $(CFLAGS) -c $<
	
check_filter_items.o : check_filter_items.c process.h defines.h
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

.PHONY : clean utest
clean :
	rm *.o *.exe

utest : unit_tests.exe
	./unit_tests.exe