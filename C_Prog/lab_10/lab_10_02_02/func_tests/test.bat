@echo off    
app.exe < func_tests/pos_0%1_in.txt > out.txt
fc out.txt func_tests/pos_0%1_out.txt  
Pause