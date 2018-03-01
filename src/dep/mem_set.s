	.globl __ffly_mem_set
	.text
/*
*	rdi - dst
*	sil - src
*	rbx - no
*/
__ffly_mem_set:
	_again:
	movb %sil, (%rdi)
	inc %rdi
	dec %rbx
	cmpq $0, %rbx
	ja _again
	ret
