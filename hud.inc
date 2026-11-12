; firmware has no font so we made one lol
; 3 by 5 font one word per letter top left is bit 14
font:   dw 0x7b6f, 0x2c97, 0x73e7, 0x73cf, 0x5bc9, 0x79cf, 0x79ef, 0x7249, 0x7bef, 0x7bcf, 0x7be4, 0x6bad, 0x79e7, 0x79cf, 0x2bed, 0x3923  ; S is literally a 5 wiggly one was unreadable

; eax letter r12d r13d top left ink colour
draw_glyph:
        push rax
        push rbx
        lea rbx,[font]
        movzx ebx,word [rbx+rax*2]
        ; top left bit to bit 31 then shl pops into carry
        shl ebx,17
        xor r14d,r14d
.row:   xor r15d,r15d
.col:   shl ebx,1
        jnc .skip
        mov dword [rbp+color],0
        mov eax,[rbp+dot]
        shr eax,2
        call .dot               ; shadow first
        mov eax,[rbp+ink]
        mov [rbp+color],eax
        xor eax,eax
        call .dot
.skip:  inc r15d
        cmp r15d,3
        jb .col
        inc r14d
        cmp r14d,5
        jb .row
        pop rbx
        pop rax
        ret
.dot:   mov r8d,r15d
        imul r8d,[rbp+dot]
        add r8d,r12d
        add r8d,eax
        mov r10d,r14d
        imul r10d,[rbp+dot]
        add r10d,r13d
        add r10d,eax
        mov r9d,r8d
        add r9d,[rbp+dot]
        mov r11d,r10d
        add r11d,[rbp+dot]
        jmp fill

; eax number r12d right edge
print_number:
        mov ecx,10
        xor edx,edx
        div ecx
        push rax
        mov eax,[rbp+dot]
        shl eax,2
        sub r12d,eax
        mov eax,edx
        call draw_glyph
        pop rax
        test eax,eax
        jnz print_number
        ret

; rsi letters 16 is space 255 ends it
print_text:
        lodsb
        cmp al,255
        je .done
        cmp al,16
        je .next
        movzx eax,al
        call draw_glyph
.next:  mov eax,[rbp+dot]
        shl eax,2
        add r12d,eax
        jmp print_text
.done:  ret

draw_hud:
        mov dword [rbp+fog],0
        mov r13d,[rbp+dot]
        shl r13d,1
        mov dword [rbp+ink],0xffffff
        mov r12d,[rbp+dot]
        imul r12d,r12d,26
        mov eax,[rbp+camz]
        shr eax,4               ; score is just how far you got
        call print_number
        mov dword [rbp+ink],0xffd21f
        mov r12d,[rbp+width]
        sub r12d,r13d
        mov eax,[rbp+coins]
        jmp print_number

press_space:
        mov dword [rbp+fog],0
        mov dword [rbp+ink],0xffffff
        mov eax,[rbp+dot]
        imul eax,eax,43
        shr eax,1
        mov r12d,[rbp+center]
        sub r12d,eax
        mov r13d,[rbp+horizon]
        shr r13d,1
        lea rsi,[press]
        call print_text
        jmp show

