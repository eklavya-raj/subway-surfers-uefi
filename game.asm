        bits 64
        default rel

%include "pe.inc"
%include "const.inc"

entry:
        lea rbp,[code_end+128]
        ; no section so nobody zeroes this we do it
        lea rdi,[rbp-128]
        mov ecx,vars_end+128
        xor eax,eax
        rep stosb
        ; stack 16 aligned before firmware calls or we cooked
        and rsp,-16
        ; 32 bytes shadow space plus extra args
        sub rsp,96
        ; rdx system table boot services at 0x60
        mov rbx,[rdx+0x60]
        mov [rbp+boot],rbx
        mov rax,[rdx+0x30]
        mov [rbp+keyboard],rax

        xor ecx,ecx
        xor edx,edx
        xor r8,r8
        xor r9,r9
        ; watchdog off or firmware reboots us in 5 min nah
        call [rbx+0x100]

        ; firmware pls give graphics protocol
        lea rcx,[gop_guid]
        xor edx,edx
        lea r8,[rbp+gop]
        call [rbx+0x140]

        ; mode then info whatever size it already is
        mov rax,[rbp+gop]
        mov rax,[rax+0x18]
        mov rax,[rax+8]
        mov ecx,[rax+4]
        mov [rbp+width],ecx
        mov edx,ecx
        shr edx,1
        mov [rbp+center],edx
        mov eax,[rax+8]
        mov [rbp+height],eax
        imul edx,ecx,4
        ; pitch in bytes not pixels
        mov [rbp+pitch],edx
        imul edx,eax
        mov ecx,eax
        imul ecx,ecx,5          ; horizon at 5 sixteenths of the height
        shr ecx,4
        mov [rbp+horizon],ecx
        lea ecx,[rax*3]
        shr ecx,2
        mov [rbp+focal],ecx
        mov ecx,eax
        shr ecx,7               ; font pixel is height over 128
        mov [rbp+dot],ecx

        mov ecx,2
        ; back buffer bc direct draw flickers
        lea r8,[rbp+buffer]
        call [rbx+0x40]

        call clock
        ; tsc ticks per 60th of a sec stall 100 ms divide by 6
        mov r12,rax
        ; microseconds so this is 100 ms
        mov ecx,100000
        call [rbx+0xf8]
        call clock
        sub rax,r12
        xor edx,edx
        mov ecx,6
        div rcx
        mov [rbp+step],rax

        mov [rbp+stack],rsp         ; crash lands here clean stack
        call draw_scene
crash:  mov rsp,[rbp+stack]
        call wait_space
        lea rdi,[rbp+state]
        mov ecx,state_end-state
        xor eax,eax
        rep stosb
        call clock
        mov [rbp+last],rax

; catch up max 4 steps then draw once
main:   call read_keys
        call clock
        sub rax,[rbp+last]
        xor edx,edx
        div qword [rbp+step]
        test eax,eax
        jz main
        mov ecx,eax
        imul rax,[rbp+step]
        add [rbp+last],rax
        cmp ecx,4
        jbe .steps
        mov ecx,4
.steps: push rcx
        call update
        pop rcx
        loop .steps
        call draw_scene
%ifdef FPS
        inc dword [rbp+frames]
%endif
        jmp main

draw_scene:
        call draw_sky
        call draw_ground
        call draw_objects
        call draw_runner
        call draw_hud
        jmp show

clock:  rdtsc
        shl rdx,32
        or rax,rdx
        ret

%include "video.inc"
%include "sky.inc"
%include "ground.inc"
%include "player.inc"
%include "draw.inc"
%include "course.inc"
%include "hud.inc"
%include "data.inc"

code_end:

; rbp 128 into the vars so each is one byte away
        absolute -128
%include "vars.inc"
vars_end:
image_end equ code_end+((vars_end+128+3) & ~3)
