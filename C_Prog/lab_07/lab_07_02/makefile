CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

app.exe : main.o input.o output.o process.o
	gcc -o $@ $^

unit_tests.exe : check_main.o check_mysort.o check_key.o process.o
	gcc -o $@ $^ -lcheck

check_mysort.o : check_mysort.c process.h
	gcc $(CFLAGS) -c $<

check_key.o : check_key.c process.h defines.h
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

.PHONY : clean
clean :
	rm *.o *.exe