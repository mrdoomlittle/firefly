	.globl __ffly_mem_set
	.text
/*
*	rdi - dst
*	sil - src
*	rbx - no
*/
__ffly_mem_set:
.L0:
	movb %sil, (%rdi)
	inc %rdi
	dec %rbx
	cmpq $0, %rbx
	ja .L0
	ret
