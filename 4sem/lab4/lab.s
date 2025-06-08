; ЛАБА 4

BITS 64

%define _read 0
%define _write 1
%define _open_file 2
%define _exit 60

extern printf
extern scanf
extern fopen
extern fclose
extern fprintf

SECTION .data

sqrtFmt db "By √_func: √(%.9g) = %.9g", 10, 0
TaylorFmt db "By Taylor: √(%.9g) = %.9g", 10, 0
clrFmt db "%*[^", 10, "]", 0
inputFmt db "%f", 0
outputFmt db "%d. %.9g", 10, 0

filename db "output", 0
modeW db 'w', 0
modeA db 'a', 0

num dd 0.0
acc dd 1.0
one dd 1
min2n dq -1
buf dd 0
fdesk dq 0

inputX_str db "Input float (-1 <= x <= 1):", 10, 0
inputAcc_str db "Input accuracy (x > 0):", 10, 0

SECTION .text

readX_input:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    push rbx
    
    .loop:
        mov rdi, inputX_str
        call printf
    
        mov rdi, inputFmt
        mov rsi, num
        call scanf
        
        cmp eax, -1
        je exitProgram
        
        cmp rax, 1
        jne .clearBuffer
        
        movss xmm0, [num]
        mov ebx, 1
        cvtsi2ss xmm1, ebx
        comiss xmm0, xmm1
        ja .loop
        
        mov ebx, -1
        cvtsi2ss xmm1, ebx
        comiss xmm0, xmm1
        jb .loop
    
    pop rbx
    add rsp, 8
    leave
    ret
    
    .clearBuffer:
        ; Очистка буфера ввода
        mov rdi, clrFmt
        call scanf
        jmp .loop

readAcc_input:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    push rbx
    
    .loop:
        mov rdi, inputAcc_str
        call printf
    
        mov rdi, inputFmt
        mov rsi, acc
        call scanf
        
        cmp eax, -1
        je exitProgram
        
        cmp rax, 1
        jne .clearBuffer
        
        movss xmm0, [acc]
        mov ebx, 0
        cvtsi2ss xmm1, ebx
        comiss xmm0, xmm1
        jbe .loop

    pop rbx
    add rsp, 8
    leave
    ret
    
    ; Очистка буфера ввода
    .clearBuffer:
        mov rdi, clrFmt
        call scanf
        jmp .loop

sqrt_1plusX:
    push rbp
    mov rbp, rsp
    
    movss xmm0, [num]
    cvtsi2ss xmm1, [one]
    addss xmm0, xmm1
    sqrtss xmm1, xmm0

    ; convert to double (из-за printf)
    cvtss2sd xmm0, xmm0
    cvtss2sd xmm1, xmm1
    
    mov rdi, sqrtFmt
    call printf
    
    leave
    ret

createFile:
    push rbp
    mov rbp, rsp
    
    mov rdi, filename
    mov rsi, modeW
    call fopen
    
    test rax, rax
    jz openFileError
    
    mov rdi, rax
    call fclose

    leave
    ret

openFileError:
    mov rax, _exit
    mov rdi, 1
    syscall

openFile:
    push rbp
    mov rbp, rsp
    
    mov rdi, filename
    mov rsi, modeA
    call fopen

    test rax, rax
    jz openFileError
    
    mov [fdesk], rax

    leave
    ret

writeNumToFile:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    movaps [rsp], xmm1
    movaps [rsp+16], xmm7
    
    mov r15, r8
    
    mov rdi, [fdesk]        ; Передаем дескриптор файла
    mov rsi, outputFmt      ; Указатель на формат строки
    mov rdx, r8             ; n
    movss xmm0, xmm1        ; En
    cvtss2sd xmm0, xmm0
    call fprintf            ; Вызываем fprintf
    
    mov r8, r15
    movss xmm0, [num]
    movss xmm6, [acc]
    movaps xmm1, [rsp]
    movaps xmm7, [rsp+16]
    add rsp, 32
    
    leave
    ret

sqrtByTaylorSeries:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    push rbx
    
    mov r8, 1       ; n = counter
    
    movss xmm0, [num]       ; в xmm0 - x
    cvtsi2ss xmm1, [one]    ; в xmm1 - En (n-ый элемент)
    movss xmm6, [acc]       ; в xmm6 - точность
    movss xmm7, xmm1        ; в xmm7 - результат
    
    ; xmm2 - buffer
    .loop:
        cmp ebx, -1
        je .negf2
        
        .m:
            mov ebx, r8d       ; 2n * 2n-1
            add ebx, r8d
            mov eax, ebx
            dec ebx            ; в rbx 2n-1
            mul rbx
            
            mulss xmm1, xmm0   ; En * x
            cvtsi2ss xmm2, eax
            mulss xmm1, xmm2   ; En * (2n * 2n-1)
            cvtsi2ss xmm2, r8d
            divss xmm1, xmm2   ; En / n
            divss xmm1, xmm2   ; En / n
            mov eax, 4
            cvtsi2ss xmm2, eax
            divss xmm1, xmm2   ; En / 4
            neg ebx
            cvtsi2ss xmm2, ebx
            divss xmm1, xmm2   ; En / (1-2n)
            add ebx, 2
            cvtsi2ss xmm2, ebx
            mulss xmm1, xmm2   ; En * (1 - 2(n-1))
            mov ebx, -1
            cvtsi2ss xmm2, ebx
            mulss xmm1, xmm2   ; En * (-1)
            
            call writeNumToFile
            
            addss xmm7, xmm1
            inc r8
            
            mov ebx, 0
            cvtsi2ss xmm2, ebx
            comiss xmm1, xmm2
            jb .negf
        
        .checkAcc:
            comiss xmm1, xmm6
            ja .loop
    
    ;printFmtTaylor
    cvtsi2ss xmm1, [one]
    addss xmm0, xmm1
    movss xmm1, xmm7
    
    cvtss2sd xmm0, xmm0
    cvtss2sd xmm1, xmm1
    
    mov rdi, TaylorFmt
    call printf
    
    ;closeFile
    mov rdi, [fdesk]
    call fclose
    
    pop rbx
    add rsp, 8
    leave
    ret
    
    .negf:
        mov ebx, -1
        cvtsi2ss xmm2, ebx
        mulss xmm1, xmm2
        jmp .checkAcc
    
    .negf2:
        mulss xmm1, xmm2
        jmp .m


GLOBAL _start
_start:
    call createFile
    call openFile
    call readX_input
    call readAcc_input
    call sqrt_1plusX
    call sqrtByTaylorSeries

    call exitProgram
      
exitProgram:
    mov rax, _exit
    mov rdi, 0
    syscall