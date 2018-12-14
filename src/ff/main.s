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
	call $ffly_sched_init
	call $ffly_resv_init
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
