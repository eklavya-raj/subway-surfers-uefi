update:
        inc dword [rbp+tick]
        mov eax,[rbp+tick]
        shr eax,9               ; speed up every 512 steps caps at 22
        add eax,start_speed
        cmp eax,top_speed
        jbe .go
        mov eax,top_speed
.go:    mov [rbp+speed],eax
        add [rbp+camz],eax

        mov eax,[rbp+lane]
        shl eax,6
        sub eax,[rbp+px]
        ; slide half the gap each step
        mov ecx,eax
        sar eax,1
        jnz .ease
        mov eax,ecx             ; last unit or he gets stuck at 63 lol
.ease:  add [rbp+px],eax
        mov eax,[rbp+px]
        sar eax,2
        neg eax                 ; camera follows three quarters of the way
        add eax,[rbp+px]
        mov [rbp+camx],eax

        cmp dword [rbp+roll],0
        je .fall
        dec dword [rbp+roll]
.fall:  sub dword [rbp+vy],gravity
        mov eax,[rbp+vy]
        add [rbp+pyf],eax
        call collide
        mov eax,[rbp+floor]
        shl eax,4
        cmp [rbp+pyf],eax
        jg .air
        mov [rbp+pyf],eax
        mov dword [rbp+vy],0
.air:   ret

; whats under him rn
collide:
        mov eax,[rbp+camz]
        add eax,runner_z
        mov r8d,eax
        and r8d,511
        mov ebx,[rbp+lane]
        call object_at
        xor r9d,r9d             ; nothing there means ground
        cmp byte [rbp+kind],4
        jne .wall
        mov eax,r8d
        and eax,63
        cmp eax,[rbp+speed]
        jae .done
        inc dword [rbp+coins]
        jmp .done
.wall:  movzx eax,byte [r13]
        shl eax,1
        add eax,[rbp+speed]         ; or fast runs phase through barriers
        cmp r8d,eax
        jae .done
        mov eax,r8d
        call height_at
        cmp dword [rbp+roll],0
        je .solid
        cmp byte [r13+1],0
        jne .done
.solid: cmp byte [rbp+kind],3
        jne .check
        cmp edx,56              ; slope always catches you W ramp
        jb .stand
.check: mov eax,[rbp+pyf]
        sar eax,4
        add eax,6
        cmp eax,edx
        jl crash                ; skill issue
.stand: mov r9d,edx
.done:  mov [rbp+floor],r9d
        ret

wait_space:
        ; 40 keeps the stack lined up again
        sub rsp,40
        call press_space
.wait:  mov rcx,[rbp+keyboard]
        lea rdx,[rbp+key]
        call [rcx+8]
        test rax,rax
        jnz .wait
        cmp word [rbp+key+2],' '
        jne .wait
        add rsp,40
        ret

read_keys:
        sub rsp,40
.next:  mov rcx,[rbp+keyboard]
        lea rdx,[rbp+key]
        call [rcx+8]
        test rax,rax
        jnz .done
        movzx eax,word [rbp+key]    ; scan codes 1 up 2 down 3 right 4 left
        cmp eax,4
        jne .right
        cmp dword [rbp+lane],-1
        je .next
        dec dword [rbp+lane]
.right: cmp eax,3
        jne .up
        cmp dword [rbp+lane],1
        je .next
        inc dword [rbp+lane]
.up:    cmp eax,1
        jne .down
        mov ecx,[rbp+floor]
        shl ecx,4
        cmp [rbp+pyf],ecx
        jne .next
        mov dword [rbp+vy],jump_power
.down:  cmp eax,2
        jne .next
        mov dword [rbp+roll],roll_time
        mov dword [rbp+vy],-135     ; roll in the air slams you down
        jmp .next
.done:  add rsp,40
        ret
