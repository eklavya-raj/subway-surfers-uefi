; eax distance sets size and fog
; closer means bigger fixed point again
set_depth:
        mov ecx,eax
        mov eax,[rbp+focal]
        shl eax,16
        xor edx,edx
        div ecx
        mov [rbp+scale],eax
        shr ecx,5
        cmp ecx,128
        jbe .fog
        mov ecx,128
.fog:   mov [rbp+fog],ecx
        ret

project_x:
        sub eax,[rbp+camx]
        imul eax,[rbp+scale]
        sar eax,16
        add eax,[rbp+center]
        ret

project_y:
        neg eax
        add eax,cam_h
        imul eax,[rbp+scale]
        sar eax,16
        add eax,[rbp+horizon]
        ret

; eax ebx left right ecx edx bottom top in world units
rect:
        call project_x
        mov r8d,eax
        mov eax,ebx
        call project_x
        mov r9d,eax
        mov eax,edx
        call project_y
        mov r10d,eax
        mov eax,ecx
        call project_y
        mov r11d,eax

; r8d r9d left right r10d r11d top bottom on screen
fill:
        push rdi
        ; clip or we scribble all over memory
        xor eax,eax
        cmp r8d,eax
        cmovl r8d,eax
        cmp r10d,eax
        cmovl r10d,eax
        mov eax,[rbp+width]
        cmp r9d,eax
        cmovg r9d,eax
        mov eax,[rbp+height]
        cmp r11d,eax
        cmovg r11d,eax
        sub r9d,r8d
        jle .done
        sub r11d,r10d
        jle .done

        mov eax,[rbp+color]
        mov edx,sky_low
        mov ecx,[rbp+fog]
        call blend
        mov edi,r10d
        imul edi,[rbp+pitch]
        add rdi,[rbp+buffer]
        lea rdi,[rdi+r8*4]
        mov r10d,[rbp+pitch]
.row:   push rdi
        mov ecx,r9d
        rep stosd
        pop rdi
        add rdi,r10
        dec r11d
        jnz .row
.done:  pop rdi
        ret

; rsi is the list ends with 0
; colour left right bottom top 0 ends it
draw_rects:
        push rsi
        push rbx
.next:  movzx eax,byte [rsi]
        test eax,eax
        jz .done
        cmp eax,paint
        jne .table
        mov eax,[rbp+obj_color]
        jmp .set
.table: lea rdx,[colors]
        mov eax,[rdx+rax*4-4]
.set:   mov [rbp+color],eax
        movsx eax,byte [rsi+1]
        add eax,[rbp+obj_x]
        movsx ebx,byte [rsi+2]
        add ebx,[rbp+obj_x]
        movsx ecx,byte [rsi+3]
        add ecx,[rbp+base_y]
        movsx edx,byte [rsi+4]
        add edx,[rbp+base_y]
        cmp byte [rbp+outline],0
        je .draw
        mov dword [rbp+color],0     ; outline pass black one unit bigger
        dec eax
        inc ebx
        dec ecx
        inc edx
.draw:  call rect
        add rsi,5
        jmp .next
.done:  pop rbx
        pop rsi
        ret

draw_runner:
        mov eax,runner_z
        call set_depth
        mov eax,[rbp+px]
        mov [rbp+obj_x],eax
        mov eax,[rbp+floor]
        mov [rbp+base_y],eax
        lea rsi,[shadow]
        call draw_rects
        mov byte [rbp+outline],1
        call .parts
        mov byte [rbp+outline],0
.parts: mov r12d,[rbp+pyf]
        sar r12d,4
        cmp dword [rbp+roll],0
        je .legs
        sub r12d,14
        jmp .body
.legs:  mov r13d,4
        mov r14d,r13d
        mov eax,[rbp+floor]
        shl eax,4
        cmp [rbp+pyf],eax
        jne .draw
        mov r13d,[rbp+tick]
        and r13d,8              ; legs swap every 8 steps
        shr r13d,1
        mov r14d,r13d
        xor r14d,4
.draw:  lea eax,[r12+r13]
        mov [rbp+base_y],eax
        lea rsi,[left_leg]
        call draw_rects
        lea eax,[r12+r14]
        mov [rbp+base_y],eax
        lea rsi,[right_leg]
        call draw_rects
.body:  mov [rbp+base_y],r12d
        lea rsi,[body]
        jmp draw_rects
