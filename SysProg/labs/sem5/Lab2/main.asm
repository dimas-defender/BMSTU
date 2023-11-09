.386p

; far_jump на сегмент, смещение
far_jump macro label, segment  
    db	0EAh 
    dd	offset label
    dw	segment
endm

; печать строки на экран
print_str macro str 
    mov ah, 09h
    lea dx, str
    int 21h
	xor dx, dx
    mov ah, 2
    mov dl, 13
    int 21h
    mov dl, 10
    int 21h
endm

; ожидание ввода символа с клавиатуры
wait_key macro 
    push eax
    mov ah, 10h
    int 16h
    pop eax
endm

; очистить экран
clear_screen macro 
    mov	ax, 3
    int	10h
endm

; загрузка дескриптора в таблицу
load_gdt macro gdt_desc
    shl eax, 4                        
    mov word ptr gdt_desc.base_l, ax  ; загрузка младшей часть базы
    shr eax, 16                       
    mov byte ptr gdt_desc.base_m, al  ; загрузка средней части базы
    mov byte ptr gdt_desc.base_h, ah  ; загрузка старшей части базы
endm

; загрузка в gdtr лин. баз. адреса и размера GDT
init_gdtr macro reg                 ; в reg лин.адрес GDT
    mov	dword ptr gdtr + 2, reg	    ; адрес в старшие 4 байта gdtr
    mov word ptr  gdtr, gdt_size-1	; размер gdt в младшие 2 байта
    lgdt fword ptr gdtr 		    ; загрузим GDT
endm

load_idt macro idt_desc
    mov	idt_desc.offs_l, ax ; загрузить младшую часть смещения
    shr	eax, 16             
    mov	idt_desc.offs_h, ax ; загрузить старшую часть смещения
endm

init_idtr macro reg                 ; в reg полный линейный адрес IDT
    mov	 dword ptr idtr + 2, reg	; адрес в старшие 4 байта idtr
    mov  word ptr  idtr, idt_size-1	; размер idt в младшие 2 байта
endm

;перепрограммируем контроллер
set_interrupt_base macro base
	mov	al, 11h						; команда - инициализировать ведущий контроллер
	out	20h, al						; отправить команду по шине данных ведущему контроллеру

	mov	al, base					; базовый вектор установить в 32
	out	21h, al						; отправить базовый вектор ведущему контроллеру
	
	mov	al, 4						; ведомый подключён к IRQ2
	out	21h, al                     

	mov	al, 1						; указываем, что нужно будет посылать
	out	21h, al                     ; команду завершения обработчика прерывания
endm

mem_str macro 
    mov di, 0
    mov ah, 00000110b
    mov al, 'M'
    stosw
    mov al, 'e'
    stosw
    mov al, 'm'
    stosw
    mov al, 'o'
    stosw
    mov al, 'r'
    stosw
    mov al, 'y'
    stosw
    mov al, ':'
    stosw              
endm

descr struc    
    lim 	dw 0
    base_l 	dw 0
    base_m 	db 0
    attr_1	db 0
    attr_2	db 0
    base_h 	db 0
descr ends

intr struc 
    offs_l 	dw 0
    sel		dw 0
    cntr    db 0
    attr	db 0
    offs_h 	dw 0
intr ends

stack_seg segment  para stack 'STACK'
    stack_start	db	100h dup(?)
    stack_size = $-stack_start
stack_seg 	ENDS

data_seg segment para 'DATA'

    gdt_null  descr <>
    gdt_DS_4gb descr <0FFFFh, 0, 0, 10010010b, 11001111b, 0>
    gdt_CS_32bit descr <pm_code_size-1, 0, 0, 10011000b, 01000000b, 0>    
    gdt_DS_32bit descr <data_size-1, 0, 0, 10010010b, 01000000b, 0>
    gdt_SS_32bit descr <stack_size-1, 0, 0, 10010110b, 01000000b, 0>
    gdt_VB_32bit descr <3999, 8000h, 0Bh, 10010010b, 01000000b, 0>

    gdt_size = $-gdt_null
    gdtr	df 0
   
    sel_DS_4gb      equ   8   
    sel_CS_32bit    equ   16
    sel_DS_32bit    equ   24
    sel_SS_32bit    equ   32
    sel_videobuffer equ   40
    
    IDT	label byte

	trap_f intr 13 dup (<0, sel_CS_32bit, 0, 10001111b, 0>) 
	trap_13 intr <0, sel_CS_32bit, 0, 10001111b, 0>
	trap_s intr 18 dup (<0, sel_CS_32bit, 0, 10001111b, 0>) 
    int_timer intr <0, sel_CS_32bit, 0, 10001110b, 0>
    int_keyboard intr	<0, sel_CS_32bit, 0, 10001110b, 0> 

    idt_size = $-IDT

    idtr df 0
    idtr_backup dw	3FFh, 0, 0

    mask_master	db 0
    mask_slave	db 0
        
	ascii	db 0, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 0, 0
			db 81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 91, 93, 0, 0, 65, 83
			db 68, 70, 71, 72, 74, 75, 76, 59, 39, 96, 0, 92, 90, 88, 67
			db 86, 66, 78, 77, 44, 46, 47

    flag_enter_pr	    db 0				 
    cnt_time	        dd 0
    syml_pos     dd 80 * 2 * 5
	color_black = 0h
	color = 7h ; 17h - серый на синем фоне

    msg_in_rm   db 27, '[32;20mNow in Real Mode. ', 27, '[0m$'
    msg_move_pm db 27, '[32;20mTo enter Protected Mode press any key!', 27, '[0m$'
    msg_out_pm  db 27, '[32;20mNow in Real Mode again! ', 27, '[0m$'

    data_size = $-gdt_null
data_seg ends

pm_seg segment para public 'CODE' use32
    assume cs:pm_seg, ds:data_seg, ss:stack_seg

pm_start:
    ; загрузка селекторов в сегм. регистры
    mov	ax, sel_DS_32bit 
    mov	ds, ax
    mov	ax, sel_videobuffer
    mov	es, ax
    mov	ax, sel_SS_32bit
    mov	ss, ax
    mov	eax, stack_size
    mov	esp, eax

    sti ; разрешить прерывания, запрещенные в р.р.
    mem_str
    call count_memory
    proccess:
        test flag_enter_pr, 1
        jz	proccess
        cli ; сброс флага прерывания IF = 0
        far_jump next, sel_CS_32bit
		
next:  ; возврат в реальный режим
	mov	eax, cr0
    and	al, 0FEh 	; сброс флага з.р.
    mov	cr0, eax
	
	far_jump real, rm_seg
	
		
dummy_exc proc
    iret
dummy_exc endp

exc13 proc
    pop eax
    iret
exc13 endp
    
; обработчик прерывания от системного таймера
int_time proc uses eax
    	
    mov  eax, cnt_time ; текущее количество тиков
    push eax
    mov edi, 80 * 2
    xor eax, eax
    test cnt_time, 05
    jz timer_mark
    test cnt_time, 09
    jnz skip
    
    mov al, ' '
    jmp pr
timer_mark:
    mov al, 21h  ; '!'
pr:
    mov ah, 7
    stosw		
        
skip:	
    pop eax
    inc eax ; увеличение текущего количества тиков
    mov cnt_time, eax ; сохранение 
    
    ; отправка EOI ведущему контроллеру прерываний
    mov	al, 20h 
    out	20h, al
    iretd
int_time endp

int_keyboard proc uses eax ebx edx 
    in	al, 60h             ; Получить скан-код нажатой клавиши из порта клавиатуры
    cmp	al, 1Ch 	        ; Сравниваем с кодом enter
    jne	print_value         ; Если не enter - выводим
    or flag_enter_pr, 1     ; Если enter - устанавливаем флаг возврата в р.р.
    jmp allow_handle_keyboard
    
print_value:
    cmp al, 80h  ; какой скан-код пришел: нажатой клавиши или отжатой?
    ja allow_handle_keyboard 	 ; Если отжатой, то ничего не выводим
	
	cmp al, 0Eh
	je backspace
                    
    xor ah, ah	 ; Если нажатой, то выведем на экран
    xor ebx, ebx
    mov bx, ax

    mov dl, ascii[ebx]   ; Получим ASCII код нажатой клавиши по скан-коду из таблицы
	mov al, dl
    mov ah, color
    mov ebx, syml_pos   ; Текущая позиция вывода символа
    mov es:[ebx], ax

    add syml_pos, 2
	jmp allow_handle_keyboard
	
backspace: 
    sub syml_pos, 2
	mov dl, ' '
    mov al, dl
    mov ah, color_black
    mov ebx, syml_pos
    mov es:[ebx], ax

allow_handle_keyboard:
    in	al, 61h ; получим содержимое порта
    or	al, 80h ; установкой старшего бита 
    out	61h, al ; и последующим его сбросом
    and al, 7Fh ; сообщим контроллеру о
    out	61h, al ; приёме скан-кода символа

    mov	al, 20h ; EOI ведущему контроллеру прерываний
    out	20h, al

    iretd
int_keyboard endp

count_memory proc uses ds eax ebx 
    mov ax, sel_DS_4gb
    mov ds, ax
    
    mov ebx, 100001h 
    mov dl, 10101110b  

    mov	ecx, 0FFEFFFFEh

iterate_through_memory:
    mov dh, ds:[ebx]     ; сохраняем байт памяти
    mov ds:[ebx], dl     ; пишем в память
    cmp ds:[ebx], dl     ; проверяем - если записано то, что мы пытались записать,
                         ; то это доступная память
    jnz print_memory_counter    ; иначе мы дошли до конца памяти

    mov	ds:[ebx], dh ; восстановить байт памяти
    inc ebx          ; если удалось записать - увеличиваем счетчик
    loop iterate_through_memory

print_memory_counter:
    mov eax, ebx ; переводим в Mb
    xor edx, edx

    mov ebx, 100000h ; делим на 2^20
    div ebx

    mov ebx, 2 * 10
    call print_eax

    ret
count_memory endp
        
; вывод значения eax в видеобуффер
; в ebx позиция вывода на экран 
print_eax proc uses ecx ebx edx     
    add ebx, 10h 
    mov ecx, 8
	
	add ebx, 2
	mov dl, 'h'
	mov es:[ebx], dl
	sub ebx, 2
    
print_symbol: 
    mov dl, al
    and dl, 0Fh      
        
    cmp dl, 10
    jl add_zero_sym
    add dl, 'A' - '0' - 10 

add_zero_sym:
    add dl, '0' ; преобразуем число в цифру               
    mov es:[ebx], dl ; записать в видеобуффер
    ror eax, 4       ; циклически сдвинуть вправо
    sub ebx, 2       ; печатаем след. символ
    loop print_symbol
    ret
print_eax endp

    pm_code_size = $-pm_start
pm_seg ends

rm_seg segment para public 'CODE' use16
    assume cs:rm_seg, ds:data_seg, ss: stack_seg

start:
    mov ax, data_seg
    mov ds, ax
    mov ax, pm_seg
    mov es, ax

    print_str msg_in_rm  
    print_str msg_move_pm

    wait_key
    clear_screen 

    xor	eax, eax
    
    ; загружаем адреса сегментов в GDT
    mov ax, pm_seg
    load_gdt gdt_CS_32bit

    mov ax, data_seg
    load_gdt gdt_DS_32bit

    mov ax, stack_seg
    load_gdt gdt_SS_32bit

    mov ax, data_seg  
    shl eax, 4   
    add	eax, offset gdt_null
    init_gdtr eax

	; в eax смещение обработчика
    lea eax, es:dummy_exc
    load_idt trap_f
    
    lea eax, es:dummy_exc
    load_idt trap_s
    
    lea eax, es:exc13
    load_idt trap_13;

    lea eax, es:int_time
    load_idt int_timer ; прерывание от таймера

    lea eax, es:int_keyboard
    load_idt int_keyboard ; прерывание от клавиатуры
    
    mov ax, data_seg
    shl eax, 4
    add	eax, offset IDT; в eax линейный адрес IDT
    init_idtr eax
    
    ; для возврата в р.р. сохраним маски контроллеров прерываний
    in	al, 21h			; получить набор масок (флагов)
    mov	mask_master, al			    
    in	al, 0A1h			
    mov	mask_slave, al

    ; перепрограммируем контроллер на новый базовый вектор 32
    set_interrupt_base 32

    ; Запрет всех прерываний в ведущем контроллере, кроме IRQ0(таймер) и IRQ1(клавиатура)
    ; установка новых масок
    mov	al, 0FCh
    out	21h, al
    ; запрет всех прерываний в ведомом контроллере
    mov	al, 0FFh
    out	0A1h, al
    
    lidt fword ptr idtr ; загрузка IDT
    
    ; открытие линии A20
    in	al, 92h						
    or	al, 2						
    out	92h, al				

    cli         ; запретить маскируемые прерывания
    in	al, 70h ; и немаскируемые прерывания
    or	al, 80h
    out	70h, al

    ; Переход в защищенный режим
    mov	eax, cr0
    or eax, 1
    mov	cr0, eax

    db	66h 
    far_jump pm_start, sel_CS_32bit

real:
    mov	ax, data_seg
    mov	ds, ax          
    mov	ax, stack_seg
    mov	ss, ax
    mov	ax, stack_size
    mov	sp, ax

    set_interrupt_base 8 ; базовый вектор контроллера прерываний снова = 8 
    mov	al, mask_master ; восстановить маски контроллеров прерываний
    out	21h, al
    mov	al, mask_slave
    out	0A1h, al

    ; загружаем таблицу дескрипторов прерываний реального режима
    lidt fword ptr idtr_backup

    in	al, 70h ; разрешить немаскируемые прерывания
    and	al, 7FH
    out	70h, al
    sti         ; и маскируемые

    clear_screen
    print_str msg_out_pm
        
    mov	ax, 4C00h
    int	21h

rm_seg	ends
end start