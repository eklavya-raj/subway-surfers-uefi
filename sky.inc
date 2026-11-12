; sky gets lighter towards the horizon
draw_sky:
        xor edi,edi
.row:   mov eax,edi
        shl eax,7
        xor edx,edx
        div dword [rbp+horizon]
        mov ecx,eax
        mov eax,sky_top
        mov edx,sky_low
        call blend
        call fill_row
        inc edi
        cmp edi,[rbp+horizon]
        jb .row
        ret
