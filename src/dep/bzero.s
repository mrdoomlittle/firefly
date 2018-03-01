	.globl __ffly_bzero
	.text
/*
*	rdi - p
*	rbx - no
*/
__ffly_bzero:
	_again:
	movb $0x0, (%rdi)
	inc %rdi
	dec %rbx
	cmpq $0, %rbx
	ja _again
	ret
