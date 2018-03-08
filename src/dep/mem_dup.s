	.globl __ffly_mem_dup
	.globl __ffly_mal
	.globl __ffly_mem_cpy
	.text
/*
*	rdi - pp
*	rsi - src
*	rbx - no
*/
__ffly_mem_dup:
	push %rbp
	movq %rsp, %rbp
	sub $8, %rsp
	push %rax
	movq %rbx, -8(%rbp)

	push %rdi
	push %rsi
	push %rbx

	call __ffly_mal	

	pop %rbx
	pop %rsi
	pop %rdi

	movq %rax, (%rdi)
	movq %rax, %rdi
	call __ffly_mem_cpy
	pop %rax
	movq %rbp, %rsp
	pop %rbp
	ret
