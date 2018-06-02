	.region text
	.globl abort
abort:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rel, 0
	asb %ae, 0
	exit %ae
.l l0
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl test
test:
;save %bp
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
	outb %ae
.l l1
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
.endof
