.region text
.extern abort
.globl ts0
ts0:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rel, 0
asb %ae, 21
outb %ae
.l l0
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl main
main:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
call $ts0
.l l1
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
