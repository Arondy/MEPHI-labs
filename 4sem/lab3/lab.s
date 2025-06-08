BITS 64
 
%define _read 0
%define _write 1
%define _open_file 2
%define _exit 60
%define bufsize 13

SECTION .data

inputStr db "Input filename:", 0x0A
inputStrSize equ $ - inputStr
wordSize dw 0

fileDesk dq 0
filenameBuffer db "1", 0
enterChar db 0x0A

shiftSize dq 0
flag db 0
finalFlag db bufsize

RCE_msg db "Read from console error!", 0x0A
RFE_msg db "Read from file error!", 0x0A
OFE_msg db "Opening file error!", 0x0A
RCE_size equ $ - RCE_msg
RFE_size equ $ - RFE_msg
OFE_size equ $ - OFE_msg

SECTION .bss

;filenameBuffer resb bufsize
stringBuffer resb bufsize

SECTION .text

readConsoleInput:
    ; Пишем в консоль приглашение к вводу
    mov     rax, _write         ; "write" function number
    mov     rdi, _write         ; console write descriptior
    mov     rsi, inputStr       ; buffer address
    mov     rdx, inputStrSize   ; number of bytes to write
    syscall

    ; Читаем из консоли
    mov     rax, _read          ; "read" function number
    mov     rdi, _read          ; console read descriptior (0)
    mov     rsi, filenameBuffer ; buffer address
    mov     rdx, bufsize        ; buffer size
    syscall
    
    test    rax, rax
    js      readConsoleError
    
    ret

readFileInput:
    ; Открываем файл для чтения
    mov     rax, _open_file          ; Системный вызов open
    mov     rdi, filenameBuffer      ; Адрес строки с именем файла
    mov     rsi, _read               ; Флаг (0 - только для чтения)
    mov     rdx, 0                   ; Режим доступа
    syscall

    test    rax, rax
    js      openFileError

    mov     [fileDesk], rax
    
    ret

firstReadLoop:
    ; Читаем из файла
    mov     rdi, [fileDesk]          ; Файловый дескриптор, который мы получили при открытии файла
    mov     rax, _read               ; Системный вызов read
    mov     rsi, stringBuffer        ; Адрес буфера для записи в него
    mov     rdx, bufsize             ; Количество байт буфера
    syscall

    cmp     rax, 0
    jl      readFileError
    je      theEnd
    ret

readLoop:
    call shiftStr
    ; Читаем из файла
    mov     rdi, [fileDesk]          ; Файловый дескриптор, который мы получили при открытии файла
    mov     rax, _read               ; Системный вызов read
    mov     rsi, stringBuffer        ; Адрес буфера для записи в него
    add     rsi, bufsize
    sub     rsi, [shiftSize]
    mov     rdx, [shiftSize]          ; Количество байт буфера
    syscall
    
    cmp     rax, 0
    jl      readFileError
    je      wholeFileWasReadLoop
    ret

shiftStr:
    mov rdi, stringBuffer
    mov r8, stringBuffer
    mov rsi, [shiftSize]
    add r8, bufsize
    sub r8, rsi
    
    .loop:
        cmp rdi, r8
        jge .end
        
        mov al, [rdi + rsi]
        mov [rdi], al
        mov byte[rdi + rsi], ' '
        inc rdi
        jmp .loop

    .end:
        ret

getWordSize:
    mov     word[wordSize], 0
    mov     qword[shiftSize], 0
    mov     r14, r15
    
    .loop:
        inc     qword[shiftSize]
        
        cmp     byte[stringBuffer + r14], ' '
        jz      .spaceExit
        cmp     byte[stringBuffer + r14], 0x0A
        jz      .enterExit
        
        mov     byte[flag], 0
        inc     r14
        inc     word[wordSize]

        cmp     r14, bufsize
        jl      .loop
        
        ret

    .spaceExit:
        inc     byte[flag]
        ret
        
    .enterExit:
        mov     byte[flag], 1
        ret

changeLettersOrder:
    xor     r11, r11                    ; buf
    xor     r15, r15                    ; counter (in a string)
    
    xor     r10, r10                    ; counter (in a word)
    call    getWordSize
    cmp     word[wordSize], 1
    jle     .l4
    test    word[wordSize], 1
    jnz     .l2                         ; если нечетное
    
    .l1:
        mov     r9b, stringBuffer[r15]      ; r9b - буфер одной буквы
        xchg    stringBuffer[r15 + 1], r9b
        mov     stringBuffer[r15], r9b
        add     r15, 2
        add     r10, 2
        
    cmp     r10w, [wordSize]
    jl      .l1
    
    .l4:
        inc     r15
        test    word[wordSize], 1
        jnz     .l3                          ; если нечетное
        ret
    
    .l2:
        inc  r10
        jmp  .l1
        
    .l3:
        inc  r15
        ret

printDelimeter:
    cmp byte[flag], 1
    jne .exit
    
    mov     rax, _write
    mov     rdi, _write
    mov     rsi, stringBuffer
    add     rsi, r14
    mov     rdx, 1
    syscall
    
    .exit:
        ret

printEnter:
    mov     rax, _write         ; "write" function number
    mov     rdi, _write         ; console write descriptior
    mov     rsi, enterChar      ; buffer address
    mov     rdx, 1              ; number of bytes to write
    syscall
    
    ret

printResultingString:
    mov     rax, _write
    mov     rdi, _write
    mov     rsi, stringBuffer
    movzx   rdx, word[wordSize]
    syscall
    
    ret

readConsoleError:
    mov     rax, _write
    mov     rdi, _write
    mov     rsi, RCE_msg
    mov     rdx, RCE_size
    syscall
    
    mov     rax, _exit
    mov     rdi, 1
    syscall

openFileError:
    mov     rax, _write
    mov     rdi, _write
    mov     rsi, OFE_msg
    mov     rdx, OFE_size
    syscall

    mov     rax, _exit
    mov     rdi, 2
    syscall

readFileError:
    mov     rax, _write
    mov     rdi, _write
    mov     rsi, RFE_msg
    mov     rdx, RFE_size
    syscall
    
    mov     rax, _exit
    mov     rdi, 3
    syscall

processAndPrint:
    call changeLettersOrder
    call printResultingString
    call printDelimeter
    ret

GLOBAL _start

_start:
    ;call readConsoleInput
    ;call printEnter
    call readFileInput
    call firstReadLoop
    
    .loop:
        call processAndPrint
        call readLoop
        jmp  .loop
    
    wholeFileWasReadLoop:
        call processAndPrint
        call shiftStr
        
        mov  rax, finalFlag
        mov  rbx, [shiftSize]
        sub  byte[rax], bl
        cmp  byte[rax], 0
        jg   wholeFileWasReadLoop
    
    theEnd:
        call    printEnter
        mov     rax, _exit
        xor     rdi, rdi
        syscall