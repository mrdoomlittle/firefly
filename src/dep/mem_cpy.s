	.globl __ffly_mem_cpy
	.text
/*
*	rdi - dst
*	rsi - src
*	rbx - count
*/
__ffly_mem_cpy:
	_again:	
	movb (%rsi), %al
	movb %al, (%rdi)
	incq %rdi
	incq %rsi
	decq %rbx
	cmpq $0, %rbx
	ja _again
	ret
