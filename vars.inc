boot:     resq 1
keyboard: resq 1
gop:      resq 1
buffer:   resq 1
stack:    resq 1
step:     resq 1
last:     resq 1
width:    resd 1
height:   resd 1
pitch:    resd 1
center:   resd 1
horizon:  resd 1
focal:    resd 1
scale:    resd 1
fog:      resd 1
color:    resd 1
obj_x:    resd 1
obj_z:    resd 1
obj_color: resd 1
base_y:   resd 1
field:    resd 1
bed:      resd 1
rail:     resd 1
key:      resd 1
outline:  resb 1

        alignb 4
state:
camz:     resd 1
camx:     resd 1
tick:     resd 1
speed:    resd 1
px:       resd 1
lane:     resd 1
pyf:      resd 1
vy:       resd 1
roll:     resd 1
floor:    resd 1
coins:    resd 1
state_end:
%ifdef FPS
frames:   resd 1
%endif
dot:      resd 1
ink:      resd 1
kind:     resb 1
