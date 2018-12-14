	.region text
	.globl ffly_mem_alloc
ffly_mem_alloc:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 37
	subq %rlx, %sp
	asq %rel, 8
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	std %rlx, %ael
	asq %xes, 21
	movq %bp, %rlx
	subq %xes, %rlx
	ldd %rlx, %ael
	asq %xes, 37
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 3, %rel
	asq %rel, 37
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stq %rlx, %rel
	jmp #l0
.l l0
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
	.globl ffly_mem_free
ffly_mem_free:
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
	stq %rlx, %rel
	movq %rel, %rel
	rin 4, %rel
.l l1
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
	.globl ffly_mem_read
ffly_mem_read:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 53
	subq %rlx, %sp
	asq %rel, 8
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stq %rlx, %rel
	asq %xes, 53
	movq %bp, %rlx
	subq %xes, %rlx
	ldq %rlx, %rel
	asq %rel, 24
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	std %rlx, %ael
	asq %xes, 37
	movq %bp, %rlx
	subq %xes, %rlx
	ldd %rlx, %ael
	asq %rel, 16
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stq %rlx, %rel
	asq %xes, 45
	movq %bp, %rlx
	subq %xes, %rlx
	ldq %rlx, %rel
	asq %xes, 53
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 1, %rel
.l l2
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
	.globl ffly_mem_write
ffly_mem_write:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 53
	subq %rlx, %sp
	asq %rel, 8
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stq %rlx, %rel
	asq %xes, 53
	movq %bp, %rlx
	subq %xes, %rlx
	ldq %rlx, %rel
	asq %rel, 24
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	std %rlx, %ael
	asq %xes, 37
	movq %bp, %rlx
	subq %xes, %rlx
	ldd %rlx, %ael
	asq %rel, 16
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stq %rlx, %rel
	asq %xes, 45
	movq %bp, %rlx
	subq %xes, %rlx
	ldq %rlx, %rel
	asq %xes, 53
	movq %bp, %rel
	subq %xes, %rel
	movq %rel, %rel
	rin 2, %rel
.l l3
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
.endof
