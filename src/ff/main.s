	.region text
	.extern abort
	.extern ffly_mem_alloc
	.extern ffly_mem_free
	.extern ffly_mem_read
	.extern ffly_mem_write
	.extern ffly_sched_init
	.extern ffly_resv_init
	.globl main
main:
	;save %bp
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %bp
	;push end
	movq %sp, %bp
	;place %sp at end
	asq %rlx, 2
	subq %rlx, %sp
	asq %rel, 0
	asb %ae, 0
	;load start
	asq %xes, 1
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	;load end
	asq %rel, 0
	asb %ae, 0
	;load start
	asq %xes, 2
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	;load end
.l bk
	;binary op start
	asq %rel, 0
	asq %rel, 1
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stb %rlx, %ae
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %rel
	;push end
	asq %rel, 0
	asq %rel, 0
	asq %rel, 0
	asb %ae, 1
	movq %rel, %xes
	;pop start
	stq %sp, %rel
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	addq %xes, %rel
	;binary op end
	;load start
	asq %xes, 1
	movq %bp, %rlx
	subq %xes, %rlx
	ldb %rlx, %ae
	;load end
	asq %rel, 1
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stb %rlx, %ae
outb %ae
	;if start
	;binary op start
	asq %rel, 0
	asq %rel, 1
	movq %bp, %rlx
	subq %rel, %rlx
	asq %rel, 0
	stb %rlx, %ae
	;push start
	asq %rlx, 8
	subq %rlx, %sp
	ldq %sp, %rel
	;push end
	asq %rel, 0
	asq %rel, 0
	asb %ae, 4
	movq %rel, %xes
	;pop start
	stq %sp, %rel
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	cmpq %rel, %xes
	;binary op end
je #l1
jmp #bk
.l l1
	;if end
.l l0
	movq %bp, %sp
	;reset %bp
	;pop start
	stq %sp, %bp
	asq %rlx, 8
	addq %rlx, %sp
	;pop end
	ret
.endof
