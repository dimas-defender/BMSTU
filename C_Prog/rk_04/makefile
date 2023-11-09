CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic

rk04.exe: main.o input.o output.o process.o
	gcc -o $@ $^
	
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