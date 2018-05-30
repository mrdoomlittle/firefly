	.globl __ffly_bzero
	.text
/*
*	rdi - p
*	rbx - no
*/
__ffly_bzero:
.L0:
	movb $0x0, (%rdi)
	inc %rdi
	dec %rbx
	cmpq $0, %rbx
	ja .L0
	ret
