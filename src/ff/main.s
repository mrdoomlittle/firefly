	.region text
	.extern abort
	.extern ffly_mem_alloc
	.extern ffly_mem_free
	.extern ffly_mem_read
	.extern ffly_mem_write
	.extern ffly_sched_init
	.globl main
main:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	call $ffly_sched_init
.l l0
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
.endof
