.region text
.globl abort
abort:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 8
subq %sp, %rlx, %sp
asq %rel, 0
asq %rlx, 8
subq %bp, %rlx, %rlx
stb %rlx, %ae
exit %ae
.l l0
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts0
ts0:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 32
subq %sp, %rlx, %sp
asq %rel, 0
asq %rlx, 8
subq %bp, %rlx, %rlx
stb %rlx, %ae
outb %ae
asq %rel, 0
asq %rlx, 16
subq %bp, %rlx, %rlx
stw %rlx, %el
outw %el
asq %rel, 0
asq %rlx, 24
subq %bp, %rlx, %rlx
std %rlx, %ael
outd %ael
asq %rel, 0
asq %rlx, 32
subq %bp, %rlx, %rlx
stq %rlx, %rel
outq %rel
.l l1
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl main
main:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asq %rel, 0
asb %ae, 21
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %ae
asq %rel, 0
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %ae
asq %rlx, 17
subq %bp, %rlx, %rlx
ldb %rlx, %ae
asq %rel, 0
asb %ae, 4
asq %rlx, 25
subq %bp, %rlx, %rlx
ldw %rlx, %el
asq %rel, 0
asb %ae, 6
asq %rlx, 33
subq %bp, %rlx, %rlx
ldd %rlx, %ael
asq %rel, 0
asb %ae, 8
asq %rlx, 41
subq %bp, %rlx, %rlx
ldq %rlx, %rel
call $ts0
.l l2
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
