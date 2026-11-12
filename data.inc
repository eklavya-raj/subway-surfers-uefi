shoe    equ 1
dim     equ 2
jeans   equ 3
hoodie  equ 4
shade   equ 5
skin    equ 6
hair    equ 7
cap     equ 8
black   equ 9
white   equ 10
glass   equ 11
lime    equ 12
post    equ 13
red     equ 14
gold    equ 15
gold_rim equ 16
gold_hi equ 17
orange  equ 18
pack    equ 19
strap   equ 20
cap_dark equ 21
trunk   equ 22
leaf    equ 23
leaf_hi equ 24
needle  equ 25
paint   equ 26

; colours start at 1 bc 0 ends a list
colors:
        dd 0x101010
        dd 0x2a2a30
        dd 0x2d4f8f
        dd 0xe2492f
        dd 0xb5321f
        dd 0xe0a77a
        dd 0x3b2414
        dd 0x1f57c8
        dd 0x000000
        dd 0xf4f4f4
        dd 0x1b2433
        dd 0x8bd630
        dd 0x8a8f99
        dd 0xd8322a
        dd 0xffd21f
        dd 0xc98a00
        dd 0xfff3a0
        dd 0xf08a24
        dd 0x6b4a2b
        dd 0x1c1410
        dd 0x163f91
        dd 0x5a3a20
        dd 0x1f5a26
        dd 0x2f7d32
        dd 0x174a22

; half length, bottom, top, colour, half width, front, side
objects:
        db 4,20,30,lime,26,high_bar-parts,0
        db 4,0,14,red,26,low_bar-parts,0
        db 160,0,56,0,26,train_front-parts,train_side-parts
        db 160,0,56,0,26,0,train_side-parts
        db 0,8,18,gold,7,coin-parts,0

shadow:
        db dim,-11,11,-4,0
        db 0

left_leg:
        db jeans,-6,-1,4,17
        db shoe,-7,-1,0,4
        db shade,-11,-8,14,26
        db skin,-11,-8,11,14
        db 0

right_leg:
        db jeans,1,6,4,17
        db shoe,1,7,0,4
        db shade,8,11,14,26
        db skin,8,11,11,14
        db 0

body:
        db hoodie,-7,7,16,30
        db shade,-7,-4,16,30
        db pack,-5,5,18,28
        db strap,-5,-3,26,29
        db strap,3,5,26,29
        db skin,-3,3,29,31
        db skin,-6,-5,31,34
        db skin,5,6,31,34
        db hair,-5,5,31,36
        db cap,-5,5,34,38
        db cap_dark,-6,6,33,35
        db 0

round_tree:
        db trunk,-3,3,0,26
        db leaf,-17,17,22,46
        db leaf,-13,13,18,50
        db leaf_hi,-14,6,30,46
        db leaf_hi,-10,2,46,48
        db 0

pine:
        db trunk,-2,2,0,12
        db needle,-16,16,10,20
        db needle,-12,12,20,30
        db needle,-8,8,30,40
        db needle,-4,4,40,48
        db leaf_hi,-12,-8,10,20
        db leaf_hi,-8,-4,20,30
        db leaf_hi,-4,0,30,40
        db 0

parts:  db 0
high_bar:
        db black,-27,27,19,31
        db paint,-26,26,20,30
        db white,-26,26,24,26
        db post,-24,-20,0,20
        db post,20,24,0,20
        db 0
low_bar:
        db black,-27,27,-1,15
        db paint,-26,26,0,14
        db white,-26,26,3,6
        db white,-26,26,9,12
        db 0
train_front:
        db black,-27,27,-1,52
        db black,-23,23,51,57
        db paint,-26,26,4,51
        db paint,-22,22,50,56
        db shoe,-26,26,0,5
        db glass,-20,-2,34,48
        db glass,2,20,34,48
        db white,-26,26,22,25
        db white,-22,-15,10,15
        db white,15,22,10,15
        db 0
train_side:
        db white,-26,26,22,25
        db glass,-26,26,32,46
        db 0
ramp_face:
        db orange,-20,20,-3,0
        db 0
coin:
        db black,-5,5,6,20
        db black,-7,7,8,18
        db black,-8,8,10,16
        db gold_rim,-4,4,7,19
        db gold_rim,-6,6,9,17
        db gold_rim,-7,7,11,15
        db paint,-3,3,9,17
        db paint,-5,5,11,15
        db gold_hi,-1,1,10,16
        db 0
