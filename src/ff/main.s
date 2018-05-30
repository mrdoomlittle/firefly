	.region text
	.extern abort
	.extern ffly_mem_alloc
	.extern ffly_mem_free
	.extern ffly_mem_read
	.extern ffly_mem_write
	.globl ts0
ts0:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
	call $ts1
.l l0
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ts1
ts1:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 1
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
	call $ts2
.l l1
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ts2
ts2:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 2
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
	call $ts3
.l l2
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ts3
ts3:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 3
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
	call $ts4
.l l3
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ts4
ts4:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 4
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
	call $ts5
.l l4
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl ts5
ts5:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	asq %rlx, 1
	subq %sp, %rlx, %sp
	asq %rel, 0
	asb %ae, 5
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	ldb %rlx, %ae
	asq %rel, 0
	asq %rlx, 1
	subq %bp, %rlx, %rlx
	stb %rlx, %ae
	outb %ae
.l l5
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
	.globl main
main:
;save %bp
	asq %rlx, 8
	subq %sp, %rlx, %sp
	ldq %sp, %bp
	movq %sp, %bp
	call $ts0
.l l6
	movq %bp, %sp
;reset %bp
	stq %sp, %bp
	asq %rlx, 8
	addq %sp, %rlx, %sp
	ret
.endof
