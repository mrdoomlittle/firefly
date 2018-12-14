	.region text
	.globl abort
abort:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	asq %rel, 0
	asb %ae, 0
	exit %ae
.l l0
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
	.globl test
test:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 8
	subq %rlx, %sp
	asq %rel, 8
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stb %rlx, %ae
outb %ae
.l l1
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
.endof
