.region text
.extern abort
.globl main
main:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
.l l0
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
