	.globl __ffly_round
	.text
__ffly_round:
	pushq %rbp
	movq %rsp, %rbp
	subq $20, %rsp

	movq %xmm0, -8(%rbp)
	pxor %xmm0, %xmm0

	fldl -8(%rbp)
	fstcw -10(%rbp)
	fclex

	movw $0b0001001000111111, -12(%rbp)
	fldcw -12(%rbp)
	fwait
	frndint
	fclex

	fldcw -10(%rbp)
	fstpl -20(%rbp)
	movq -20(%rbp), %xmm0

	movq %rbp, %rsp
	popq %rbp
	ret
