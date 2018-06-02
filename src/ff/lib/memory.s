	.region text
	.globl ffly_mem_alloc
ffly_mem_alloc:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 28
	subq %sp, %rlx, %sp
	asq %rel, 0
	asq %rlx, 8
	subq %bp, %rlx, %rlx
	std %rlx, %ael
	asq %rlx, 12
	subq %bp, %rlx, %rlx
	ldd %rlx, %ael
	asq %rel, 28
	subq %bp, %rel, %rel
	movq %rel, %rel
rin 3, %rel
	asq %rel, 0
	asq %rlx, 28
	subq %bp, %rlx, %rlx
	stq %rlx, %rel
	jmp #l0
.l l0
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ffly_mem_free
ffly_mem_free:
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
	stq %rlx, %rel
	movq %rel, %rel
rin 4, %rel
.l l1
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ffly_mem_read
ffly_mem_read:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 44
	subq %sp, %rlx, %sp
	asq %rel, 0
	asq %rlx, 8
	subq %bp, %rlx, %rlx
	stq %rlx, %rel
	asq %rlx, 44
	subq %bp, %rlx, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asq %rlx, 24
	subq %bp, %rlx, %rlx
	std %rlx, %ael
	asq %rlx, 28
	subq %bp, %rlx, %rlx
	ldd %rlx, %ael
	asq %rel, 0
	asq %rlx, 16
	subq %bp, %rlx, %rlx
	stq %rlx, %rel
	asq %rlx, 36
	subq %bp, %rlx, %rlx
	ldq %rlx, %rel
	asq %rel, 44
	subq %bp, %rel, %rel
	movq %rel, %rel
rin 1, %rel
.l l2
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ffly_mem_write
ffly_mem_write:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 44
	subq %sp, %rlx, %sp
	asq %rel, 0
	asq %rlx, 8
	subq %bp, %rlx, %rlx
	stq %rlx, %rel
	asq %rlx, 44
	subq %bp, %rlx, %rlx
	ldq %rlx, %rel
	asq %rel, 0
	asq %rlx, 24
	subq %bp, %rlx, %rlx
	std %rlx, %ael
	asq %rlx, 28
	subq %bp, %rlx, %rlx
	ldd %rlx, %ael
	asq %rel, 0
	asq %rlx, 16
	subq %bp, %rlx, %rlx
	stq %rlx, %rel
	asq %rlx, 36
	subq %bp, %rlx, %rlx
	ldq %rlx, %rel
	asq %rel, 44
	subq %bp, %rel, %rel
	movq %rel, %rel
rin 2, %rel
.l l3
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
.endof
