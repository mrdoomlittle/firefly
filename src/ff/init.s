	.region text
	.globl ffly_sched_init
ffly_sched_init:
	;save %bp
	;push
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 30
	subq %sp, %rlx
	asq %rel, 0
	asb %ae, 0
	asq %rel, 9
	movq %bp, %rlx
	subq %rel, %rlx
	ldb %rlx, %ae
	asq %xes, 30
	movq %bp, %rel
	subq %xes, %rel
	asq %rel, 8
	movq %bp, %rlx
	subq %rel, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asb %ae, 0
	asq %rel, 30
	movq %bp, %rlx
	subq %rel, %rlx
	ldb %rlx, %ae
	asq %xes, 29
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 5, %rel
.l l0
	movq %bp, %sp
	;reset %bp
	;pop
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	ret
	.globl ffly_resv_init
ffly_resv_init:
	;save %bp
	;push
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 30
	subq %sp, %rlx
	asq %rel, 0
	asb %ae, 1
	asq %rel, 9
	movq %bp, %rlx
	subq %rel, %rlx
	ldb %rlx, %ae
	asq %xes, 30
	movq %bp, %rel
	subq %xes, %rel
	asq %rel, 8
	movq %bp, %rlx
	subq %rel, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asb %ae, 0
	asq %rel, 30
	movq %bp, %rlx
	subq %rel, %rlx
	ldb %rlx, %ae
	asq %xes, 29
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 5, %rel
.l l1
	movq %bp, %sp
	;reset %bp
	;pop
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	ret
.endof
