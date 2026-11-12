; gop blt yeets the frame to the screen
show:
        ; 88 not 80 bc call already pushed 8
        sub rsp,88
        mov rcx,[rbp+gop]
        mov rdx,[rbp+buffer]
        mov r8d,2
        xor r9d,r9d
        ; arg 5 and up go after shadow space
        mov [rsp+32],r9
        mov [rsp+40],r9
        mov [rsp+48],r9
        mov eax,[rbp+width]
        mov [rsp+56],rax
        mov eax,[rbp+height]
        mov [rsp+64],rax
        mov [rsp+72],r9
        call [rcx+0x10]
        add rsp,88
        ret

; blend eax toward edx by ecx over 128
; sse mix 16 bits per channel so no overflow
blend:
        movd xmm0,eax
        movd xmm1,edx
        pxor xmm2,xmm2
        punpcklbw xmm0,xmm2
        punpcklbw xmm1,xmm2
        psubw xmm1,xmm0
        movd xmm3,ecx
        pshuflw xmm3,xmm3,0
        pmullw xmm1,xmm3
        psraw xmm1,7
        paddw xmm1,xmm0
        packuswb xmm1,xmm1
        movd eax,xmm1
        ret

; eax colour edi row
fill_row:
        push rdi
        imul edi,[rbp+pitch]
        add rdi,[rbp+buffer]
        mov ecx,[rbp+width]
        rep stosd
        pop rdi
        ret
