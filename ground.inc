draw_ground:
        mov edi,[rbp+horizon]
.row:   mov ebx,edi
        sub ebx,[rbp+horizon]
        inc ebx
        mov eax,cam_h
        imul eax,[rbp+focal]
        xor edx,edx
        div ebx                 ; one divide per row ez
        mov r12d,eax

        mov r13d,eax
        shr r13d,5              ; far rows fade into the sky
        cmp r13d,128
        jbe .fog
        mov r13d,128
.fog:   mov edx,[rbp+camz]
        add edx,r12d
        and edx,31              ; a sleeper every 32 units
        mov eax,gravel
        cmp edx,10
        jae .bed
        mov eax,wood
.bed:   call fogged
        mov [rbp+bed],eax
        mov eax,grass
        call fogged
        mov [rbp+field],eax
        mov eax,steel
        call fogged
        mov [rbp+rail],eax

        mov eax,cam_h
        xor edx,edx
        div ebx
        lea r14d,[rax+2]
        ; world x per pixel in 16.16 fixed point
        mov eax,cam_h<<16
        xor edx,edx
        div ebx
        mov r9d,eax
        movsxd rsi,dword [rbp+camx]
        add rsi,96
        shl rsi,16
        mov eax,[rbp+width]
        shr eax,1
        imul rax,r9
        sub rsi,rax

        mov r15d,edi
        imul edi,[rbp+pitch]
        add rdi,[rbp+buffer]
        mov ecx,[rbp+width]
.pixel: mov rax,rsi
        sar rax,16
        mov r8d,[rbp+field]
        cmp rax,192
        jae .put
        and eax,63
        sub eax,32
        cdq
        xor eax,edx
        sub eax,edx
        sub eax,20              ; rails 20 out from track middle
        cdq
        xor eax,edx
        sub eax,edx
        mov r8d,[rbp+bed]
        cmp eax,r14d
        jae .put
        mov r8d,[rbp+rail]
.put:   mov eax,r8d
        stosd
        add rsi,r9
        dec ecx
        jnz .pixel
        lea edi,[r15+1]
        cmp edi,[rbp+height]
        jb .row
        ret

fogged:
        mov edx,sky_low
        mov ecx,r13d
        jmp blend
