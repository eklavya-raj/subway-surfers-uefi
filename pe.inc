; PE header by hand no linker no problem
; no sections whole file counts as headers lowkey illegal
; e_lfanew lands on SectionAlignment both alignments 4 so headers overlap free bytes
; fields firmware never reads now hold game data
base    equ 0x400000
align_  equ 4

mz:     dw "MZ"
        dw 0
pe:     dd "PE"
        dw 0x8664               ; x64
        dw 0
press:  db 10,11,12,13,13,16,13,10,14,15,12,255 ; press space lives on the timestamp and symbol table lol
        dw opt_end-opt
        dw 0x22

opt:    dw 0x20b
lane_order:                     ; linker version and code sizes nobody reads these
        db 1,0,-1
        db -1,1,0
        db -1,0,1
        db 0,0,0,0,0
        dd entry
        dd 0
        dq base
        dd align_               ; this is also e_lfanew
        dd align_
gop_guid:                       ; os image subsystem versions unused fr
        dd 0x9042a9de
        dw 0x23dc,0x4a38
        db 0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a
        ; image size covers the vars too
        dd image_end
        dd code_end
        dd 0
        dw 10                   ; efi application
        dw 0
train_colors:                   ; stack and heap sizes also unused
        dd 0x2f6fd6,0x18a0a8,0x7a4fd0,0x3a9ee8
        dq 0,0
        dd 0
        dd 0
opt_end:

code:
