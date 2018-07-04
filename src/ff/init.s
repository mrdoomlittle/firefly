	.region text
	.globl ffly_sched_init
ffly_sched_init:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 30
	subq %rlx, %sp
	asq %rel, 0
	asb %ae, 0
	asq %xes, 9
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	asq %xes, 30
	movq %bp, %rel
	subq %xes, %rel
	asq %xes, 8
	movq %bp, %rlx
	subq %xes, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asb %ae, 0
	asq %xes, 30
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	asq %xes, 29
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 5, %rel
.l l0
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
	.globl ffly_resv_init
ffly_resv_init:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 30
	subq %rlx, %sp
	asq %rel, 0
	asb %ae, 1
	asq %xes, 9
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	asq %xes, 30
	movq %bp, %rel
	subq %xes, %rel
	asq %xes, 8
	movq %bp, %rlx
	subq %xes, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asb %ae, 0
	asq %xes, 30
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	asq %xes, 29
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 5, %rel
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
