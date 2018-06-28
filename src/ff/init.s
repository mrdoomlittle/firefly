	.region text
	.globl ffly_sched_init
ffly_sched_init:
	;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 30
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 0
	asq %rlx, 9
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 30
	subq %bp, %rel, %rel
	asq %rlx, 8
	subq %bp, %rlx, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asb %ae, 0
	asq %rlx, 30
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 29
	subq %bp, %rel, %rel
	movq %rel, %rel
	rin 5, %rel
.l l0
	movq %bp, %sp
	;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ffly_resv_init
ffly_resv_init:
	;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 30
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 1
	asq %rlx, 9
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 30
	subq %bp, %rel, %rel
	asq %rlx, 8
	subq %bp, %rlx, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asb %ae, 0
	asq %rlx, 30
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 29
	subq %bp, %rel, %rel
	movq %rel, %rel
	rin 5, %rel
.l l1
	movq %bp, %sp
	;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
.endof
