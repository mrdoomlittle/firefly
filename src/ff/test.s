	.region text
	.globl main
main:
	ret
	;save %bp
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx
	asq %rel, 0
	asb %ae, 21
	asq %rel, 1
	movq %bp, %rlx
	subq %rel, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rel, 0
	asq %rel, 1
	movq %bp, %rlx
	subq %rel, %rlx
	stb %rlx, %ae
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %rel
	asq %rel, 0
	asq %rel, 0
	asb %ae, 21
	movq %rel, %xes
	stq %sp, %rel
	asq %rlx, 8
	addq %rlx, %sp
	cmpq %rel, %xes
jne #l1
	asq %rel, 0
	asq %rel, 1
	movq %bp, %rlx
	subq %rel, %rlx
	stb %rlx, %ae
	outb %ae
.l l1
.l l0
	movq %bp, %sp
	;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	ret
_start:
	rin 0, 0
	call $main
	exit %al
.endof
