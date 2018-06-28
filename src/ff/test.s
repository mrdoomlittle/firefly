	.region text
	.globl main
main:
	;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
.l back
	asq %rel, 0
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
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
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
	asq %rel, 0
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %rel
	asq %rel, 0
	asq %rel, 0
	asb %ae, 20
	movq %rel, %xes
	stq %sp, %rel
	asq %rlx, 8
	addq %sp, %rlx, %sp
	cmpq %rel, %xes, %ll
je #l1, %ll
jmp #back
.l l1
.l l0
	movq %bp, %sp
	;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
_start:
	rin 0, 0
	call $main
	exit %al
.endof
