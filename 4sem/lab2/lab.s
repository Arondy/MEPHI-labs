bits	64

section	.data

lineNum equ 3
elNum equ 5
matrixSize equ lineNum * elNum

matrix:
    db  5, 4, 7, 8, 10
    db  9, 0, -1, -2, 9
    db  4, 2, 3, 1, 7

section .bss

ads resq lineNum            ;указатели на начала массивов
minArray resb lineNum       ;массив минимальных чисел в строке
tempMatrix resb matrixSize  ;буферная матрица


section	.text

global _start

;{ rbx, r8, r9, r10, r13, r14, r15 }
gnomeSort:                      ;вектор в r15d, кол-во эл-тов в r9d                      
    xor    ebx, ebx             ;текущий индекс
    push   r13
    
    mainLoop:
        cmp     ebx, r9d        ;if i >= n:
        jge     endLoop
        
        test    ebx, ebx        ;if i == 0:
        jz      incCount
        
        movzx   r8d, byte[r15d]      ;r8d = a[i]
        cmp     r11, 1
        jg      greater
        
        lower:
            cmp     byte[r15d-1], r8b    ;if a[i-1] <= a[i]:
            jle     incCount
            jmp     label
        greater:
            cmp     byte[r15d-1], r8b    ;if a[i-1] <= a[i]:
            jge     incCount
            
        label:
            test    r14d, r14d      ;if flag == 0 (активен)
            jz      secondArrayReorder
        
        reorder:
            xchg    byte[r15d-1], r8b
            mov     byte[r15d], r8b
        
        dec     r15d
        sub     r13d, 4
        dec     ebx
        
        jmp     mainLoop
    
    incCount:
        inc     ebx
        add     r13d, 4
        inc     r15d
        jmp     mainLoop
        
    secondArrayReorder:
        push    r14
        
        mov     r14d, [r13d]
        xchg    r13d[-4], r14d
        mov     [r13d], r14d
        
        pop     r14
        jmp     reorder
    
    endLoop:
        pop     r13
        ret

;min->eax
findMin: 
    push    rcx
    movzx   eax, byte[r15d]
    mov     ecx, r9d
    dec     ecx
    
    l1:
        inc     r15d
        cmp     byte[r15d], al  ;if a[i] < a[i-1]
        jge     l2
        movzx   eax, byte[r15d]
    
    l2:
        loop l1
    
    inc     r15d
    pop     rcx
    ret
    

_start:
    mov rbp, rsp; for correct debugging
    
    mov     ecx, lineNum
    mov     r9d, elNum
    mov     r10d, minArray
    mov     r15d, matrix
    mov     r12d, r15d          ;копия matrix для получения первого числа массивов
    mov     r13d, ads
    mov     r14d, 1
    pop     r11

    ;{ rcx, r10, r11, r12, r13 }
    ArraysSortLoop:
    ;тут была сортировка строк в матрице(((
        ;call    gnomeSort       ;{ rbx, r8, r9, r13, r14, r15 }
        ;mov     r11d, [r12d]    ;т.к. две [] нельзя mov
        ;mov     [r10d], r11d    ;заполняем minArray
        ;inc     r10d
        ;mov     [r13d], r12d    ;заполняем ads
        ;add     r13d, 4
        ;add     r12d, r9d
        
        call    findMin
        mov     [r10d], al      ;заполняем minArray
        inc     r10d
        mov     [r13d], r12d    ;заполняем ads
        add     r13d, 4
        add     r12d, r9d
        
        
    loop    ArraysSortLoop

    ;сортируем minArray вместе с ads
    mov     r9d, lineNum
    mov     r13d, ads
    xor     r14, r14
    mov     r15d, minArray
    call    gnomeSort
    
    ;{ rcx, rsi, r9, r13, r14, r15 }
    ;заполняем tempMatrix
    mov     r9d, elNum
    xor     r13, r13
    mov     r14d, ads
    mov     r15d, tempMatrix
    mov     ecx, lineNum
    
    matrixFillLoop:
        push rcx
        mov  ecx, r9d
        mov  esi, [r14d]
        
        matrixLineFillLoop:
            mov  r13b, [esi]
            mov  [r15d], r13b
            inc  esi
            inc  r15d
        
        loop matrixLineFillLoop
        add  r14d, 4
        pop  rcx
    
    loop    matrixFillLoop


    mov	     eax, 60
    mov	     edi, 0
    syscall
    
    xor rax, rax
    retq
