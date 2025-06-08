; ЛАБА 5

BITS 64

SECTION .text
global mirrorImage

mirrorImage:
    ; rdi - width
    ; rsi - height
    ; rdx - channels
    ; rcx - data (u_char*)
    
    ; r8 - y
    ; r9 - x
    ; r10 - c
    push r12
    push r13
    push r14
    push r15
    
    xor r8, r8          ; y < height
    mov r11, rdi        ; width / 2
    shr r11, 1
    mov r12, rdx        ; channels
    
    .yLoop:
        cmp r8, rsi
        je .end
        
        xor r9, r9
        .xLoop:
            cmp r9, r11
            je .yInc
            
            xor r10, r10
            .cLoop:
                cmp r10, r12
                je .xInc
                
                mov rax, r8         ; int left_index = (y * width + x) * channels + c -> r15
                mul rdi
                add rax, r9
                mul r12
                add rax, r10
                mov r15, rax
                
                mov rax, r8         ; int right_index = (y * width + (width - 1 - x)) * channels + c -> rax
                mul rdi
                add rax, rdi
                dec rax
                sub rax, r9
                mul r12
                add rax, r10
                
                mov r14b, rcx[r15]          ; unsigned char temp = data[left_index] -> r14
                mov r13b, rcx[rax]
                mov rcx[r15], r13b          ; data[left_index] = data[right_index]
                mov rcx[rax], r14b          ; data[right_index] = temp
                
                inc r10
                jmp .cLoop
            
            .xInc:
                inc r9
                jmp .xLoop
        
        .yInc:
            inc r8
            jmp .yLoop
                
    .end:
        pop r15
        pop r14
        pop r13
        pop r12
        ret