	.globl __ffly_mem_cmp
	.text
/*
*	rdi - p0
*	rsi - p1
*	rbx - no
*/
__ffly_mem_cmp:
	push %rbp
	movq %rsp, %rbp
	sub $2, %rsp
	movw $0, -2(%rbp)
	_again:
	movb (%rdi), %al
	cmpb %al, (%rsi)
	jne _j1
	incw -2(%rbp)
	cmpw %bx, -2(%rbp)
	je _j0
	inc %rdi
	inc %rsi

	jmp _again
	_j0:
	movb $0x0, %al
	jmp _sk
	_j1:
	movb $0xff, %al
	_sk:
	movq %rbp, %rsp
	pop %rbp
	ret
