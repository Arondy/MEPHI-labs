bits	64

section	.data

res:
	dq	0
a:
	dd	10
b:
	dd	999
c:
	dd	13
d:
	dw	412
e:
	dw	10

section	.text

global	_start

_start:    
    mov     rbp, rsp; for correct debugging
    
    mov     r8d, dword[a]
    mov     ebx, dword[b]
    mov     ecx, dword[c]
    movzx   r9d, word[d]
    movzx   r10d, word[e]
    
    mov     r11d, ebx;a*(b+c) -> r11 (1) 
    add     r11, rcx
    mov     eax, r8d
    mul     r11
    jo      OF       ;32*33 -> 65 бит
    mov     r11, rax
    xor     rax, rax
    
    mov     eax, r8d ;d*(e+a) -> r12 (2)
    add     rax, r10
    mul     r9
    mov     r12, rax ;16*33 -> <65 бит
    xor     rax, rax
    
    sub     r11, r12 ;(1)-(2) -> r11
    js      SF
    
    mov     ax, r9w  ;(d^2)-(c^2)*b -> r9
    mul     r9w
    shl     edx, 16
    or      eax, edx
    mov     r9d, eax
    
    mov     eax, ecx
    mul     ecx
    shl     rdx, 32
    or      rax, rdx
    mul     rbx
    jo      OF       ;64*32 -> 96 бит
    sub     r9, rax
    js      SF
    
    mov     rax, r11 ;деление
    test    r9, r9   ;проверка на 0
    jz      zero_div
    idiv    r9
    
    
    mov     [res], rax
    
    mov	    eax, 60
    mov	    edi, 0
    syscall
    
zero_div:
    mov	    eax, 60
    mov	    edi, 1
    syscall

OF:
    mov	    eax, 60
    mov	    edi, 2
    syscall
    
SF:
    mov	    eax, 60
    mov	    edi, 3
    syscall