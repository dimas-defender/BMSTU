CFLAGS := -std=c99 -Wall -Werror

func.dll : func.o
	gcc -o $@ -shared $^ -Wl,--subsystem,windows
	
func.o : func.c func.h
	gcc $(CFLAGS) -D EXPORT -c $<
	
.PHONY : clean
clean :
	rm *.o *.exe *.dll