	.globl __ffly_cos
	.text
__ffly_cos:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp

	movq %xmm0, -8(%rbp)
	pxor %xmm0, %xmm0

	fldl -8(%rbp)
	fwait
	fcos

	fstpl -16(%rbp)
	movq -16(%rbp), %xmm0

	movq %rbp, %rsp
	popq %rbp
	ret
