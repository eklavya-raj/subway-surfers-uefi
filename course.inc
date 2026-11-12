; eax distance ebx lane row comes back in r13
object_at:
        shr eax,9
        mov edx,eax
        cmp eax,3               ; first 3 slots free coins lil warmup
        jb .coin
        imul edx,edx,0x9e3779b1 ; golden ratio hash dont ask
        mov eax,edx
        shr eax,16
        imul eax,eax,3
        shr eax,16
        dec eax
        cmp eax,ebx
        je .coin
        lea ecx,[rbx*2+12]
        mov eax,edx
        shr eax,cl
        and eax,3
        jmp .row
.coin:  mov eax,4
.row:   mov [rbp+kind],al      ; kind is 0 bar 1 barrier 2 train 3 ramp 4 coins
        ; 7 byte rows change this if the table grows
        imul eax,eax,7
        lea r13,[objects]
        add r13,rax
        ret

; r13 row eax how far in height comes back in edx
height_at:
        movzx edx,byte [r13+2]
        cmp byte [rbp+kind],3
        jne .done
        shr eax,2
        cmp eax,edx
        jae .done
        mov edx,eax
.done:  ret

; far slots first near ones paint over
draw_objects:
        mov r12d,[rbp+camz]
        and r12d,-512
        add r12d,9*512
.slot:  call draw_trees
        mov r14d,3
.lane:  mov eax,[rbp+lane]
        lea eax,[rax*2+rax+6]
        sub eax,r14d
        ; rip relative cant index learned that the hard way
        lea rdx,[lane_order]
        movsx ebx,byte [rdx+rax]
        call draw_lane
        dec r14d
        jnz .lane
        sub r12d,512
        mov eax,r12d
        add eax,512
        cmp eax,[rbp+camz]
        jg .slot
        ret

; 4 trees a side hash picks the rest
draw_trees:
        mov r14d,3
.tree:  mov eax,r14d
        shl eax,7
        add eax,r12d
        sub eax,[rbp+camz]
        mov [rbp+obj_z],eax
        cmp eax,40
        jl .next
        call set_depth
        mov eax,r12d
        shr eax,7
        add eax,r14d
        imul eax,eax,0x9e3779b1
        mov ebx,eax
        shr eax,25
        add eax,130
        mov [rbp+obj_x],eax
        lea rsi,[round_tree]
        test ebx,0x10000
        jz .kind
        lea rsi,[pine]
.kind:  call draw_rects
        neg dword [rbp+obj_x]
        lea rsi,[round_tree]
        test ebx,0x20000
        jz .left
        lea rsi,[pine]
.left:  call draw_rects
.next:  dec r14d
        jns .tree
        ret

draw_lane:
        mov eax,r12d
        call object_at
        mov eax,ebx
        shl eax,6
        mov [rbp+obj_x],eax
        mov eax,r12d
        sub eax,[rbp+camz]
        mov [rbp+obj_z],eax
        movzx eax,byte [r13+3]
        test eax,eax
        jz .train
        lea rdx,[colors]
        mov eax,[rdx+rax*4-4]
        jmp .set
.train: shr edx,24
        add edx,ebx
        and edx,3
        lea rax,[train_colors]
        mov eax,[rax+rdx*4]
.set:   mov [rbp+obj_color],eax
        cmp byte [rbp+kind],4
        jne draw_box
        mov r15d,8
.coin:  cmp dword [rbp+obj_z],runner_z ; coins behind you dont get drawn
        jl .next
        call draw_box
.next:  add dword [rbp+obj_z],64
        dec r15d
        jnz .coin
        ret

; back to front one slice at a time
draw_box:
        push r15
        movzx r15d,byte [r13]
        shl r15d,1
        add r15d,[rbp+obj_z]
.slice: cmp r15d,[rbp+obj_z]
        jle .front
        call draw_slice
        mov eax,r15d
        shr eax,6               ; slices get further apart far away
        inc eax
        sub r15d,eax
        jmp .slice
.front: mov eax,[rbp+obj_z]
        cmp eax,40
        jl .done
        call set_depth
        movzx eax,byte [r13+5]
        call parts_at
.done:  pop r15
        ret

draw_slice:
        cmp r15d,40
        jl .skip
        mov eax,r15d
        call set_depth
        mov eax,r15d
        sub eax,[rbp+obj_z]
        push rax
        call height_at
        pop rax
        mov r8d,edx
        mov ecx,[rbp+obj_color]
        cmp byte [rbp+kind],3
        jne .shade
        cmp eax,ramp_length
        jae .shade
        mov dword [rbp+color],ramp_side
        mov r9d,ramp_side
        mov [rbp+base_y],r8d        ; running surface sits on top of this slice
        mov eax,ramp_face-parts
        jmp .walls
; side darker roof lighter one colour
.shade: mov eax,ecx
        xor edx,edx
        mov ecx,44
        call blend
        mov [rbp+color],eax
        mov eax,[rbp+obj_color]
        mov edx,0xffffff
        mov ecx,40
        call blend
        mov r9d,eax
        movzx eax,byte [r13+6]
.walls: push rax
        push r9
        push r8
        mov eax,[rbp+obj_x]
        movzx ebx,byte [r13+4]
        sub eax,ebx
        lea ebx,[rax+rbx*2]
        movzx ecx,byte [r13+1]
        pop rdx
        push rdx
        sub edx,5
        call rect
        pop rdx
        pop rcx
        mov [rbp+color],ecx
        lea ecx,[rdx-5]
        mov eax,[rbp+obj_x]
        movzx ebx,byte [r13+4]
        sub eax,ebx
        lea ebx,[rax+rbx*2]
        add eax,4               ; narrower roof so the top looks round
        sub ebx,4
        call rect
        pop rax
        call parts_at
        mov dword [rbp+base_y],0
.skip:  ret

; eax is the list offset from parts
parts_at:
        test eax,eax
        jz .none
        lea rsi,[parts]
        add rsi,rax
        jmp draw_rects
.none:  ret
