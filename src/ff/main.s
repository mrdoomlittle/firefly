.region text
.globl main
main:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 20
subq %sp, %rlx, %sp
asq %rel, 0
asb %ae, 2
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rlx, 20
subq %bp, %rlx, %rel
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rel, 0
asb %ae, 0
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
addq %rel, %xes, %rel
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
ldb %xes, %ea
asq %rel, 0
asb %ae, 3
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rlx, 20
subq %bp, %rlx, %rel
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rel, 0
asb %ae, 1
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
addq %rel, %xes, %rel
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
ldb %xes, %ea
asq %rel, 0
asq %rel, 0
asq %rlx, 20
subq %bp, %rlx, %rel
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rel, 0
asb %ae, 1
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
addq %rel, %xes, %rel
stb %rel, %ae
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rlx, 20
subq %bp, %rlx, %rel
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rel, 0
asb %ae, 0
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
addq %rel, %xes, %rel
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
ldb %xes, %ea
asq %rel, 0
asq %rel, 0
asq %rlx, 20
subq %bp, %rlx, %rel
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rel, 0
asb %ae, 0
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
addq %rel, %xes, %rel
stb %rel, %ae
outb %ae
asq %rel, 0
asq %rel, 0
asq %rlx, 20
subq %bp, %rlx, %rel
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %rel
asq %rel, 0
asq %rel, 0
asq %rel, 0
asb %ae, 1
movq %rel, %xes
stq %sp, %rel
asq %rlx, 8
addq %sp, %rlx, %sp
addq %rel, %xes, %rel
stb %rel, %ae
outb %ae
.l l0
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
