CFLAGS := -std=c99 -Wall -Werror

app_stat.exe : main_stat.o libstat.a
	gcc -o $@ $^
	
app_dll.exe : main_dll.o lib.dll
	gcc -o $@ $^

app_dload.exe : dload/main.c lib.dll
	gcc $(CFLAGS) -o $@ $<
	
unit_tests.exe : check_main.o check_mysort.o check_key.o libstat.a
	gcc -o $@ $^ -lcheck

check_mysort.o : utest/check_mysort.c stat/process.h
	gcc $(CFLAGS) -c $<

check_key.o : utest/check_key.c stat/process.h stat/defines.h
	gcc $(CFLAGS) -c $<

check_main.o : utest/check_main.c utest/suites.h
	gcc $(CFLAGS) -c $<
	
libstat.a : input_stat.o output_stat.o process_stat.o
	ar rc $@ $^

lib.dll : input_dll.o output_dll.o process_dll.o
	gcc -o $@ -shared $^ -Wl,--subsystem,windows

input_stat.o : stat/input_stat.c stat/input.h stat/defines.h
	gcc $(CFLAGS) -c $<

output_stat.o : stat/output_stat.c stat/output.h stat/defines.h
	gcc $(CFLAGS) -c $<

process_stat.o : stat/process_stat.c stat/process.h stat/defines.h
	gcc $(CFLAGS) -c $<

main_stat.o : stat/main_stat.c stat/input.h stat/output.h stat/process.h stat/defines.h
	gcc $(CFLAGS) -c $<
	
input_dll.o : dll/input_dll.c dll/input.h dll/defines.h
	gcc $(CFLAGS) -D EXPORT -c $<

output_dll.o : dll/output_dll.c dll/output.h dll/defines.h
	gcc $(CFLAGS) -D EXPORT -c $<

process_dll.o : dll/process_dll.c dll/process.h dll/defines.h
	gcc $(CFLAGS) -D EXPORT -c $<

main_dll.o : dll/main_dll.c dll/input.h dll/output.h dll/process.h dll/defines.h
	gcc $(CFLAGS) -c $<

.PHONY : clean utest
clean :
	rm *.o *.exe *.a *.dll

utest : unit_tests.exe
	./unit_tests.exe