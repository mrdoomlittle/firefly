	.globl __ffly_sqr
	.text
__ffly_sqr:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	movq -8(%rbp), %xmm0

	fldl -8(%rbp)
	fwait
	fmul %st(0)

	fstpl -8(%rbp)
	movq -8(%rbp), %xmm0

	movq %rbp, %rsp
	popq %rbp
	ret
