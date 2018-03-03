	.globl __ffly_mem_cpy
	.text
/*
*	rdi - dst
*	rsi - src
*	rbx - count
*/
__ffly_mem_cpy:
	push %rbp
	mov %rsp, %rbp
	push %rax
	_again:	
	movb (%rsi), %al
	movb %al, (%rdi)
	inc %rdi
	inc %rsi
	dec %rbx
	cmpq $0, %rbx
	ja _again
	pop %rax
	mov %rbp, %rsp
	pop %rbp
	ret
