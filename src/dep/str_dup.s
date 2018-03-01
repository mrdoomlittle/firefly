	.globl __ffly_str_dup
	.globl __ffly_mem_dup
	.globl __ffly_str_len
	.text
/*
*	rdi - src
*/
__ffly_str_dup:
	push %rdi
	call __ffly_str_len
	pop %rdi
	
	push %rbp
	movq %rsp, %rbp
	sub $8, %rsp

	movq %rdi, %rsi
	movq %rax, %rbx
	movq %rbp, %rdi
	sub $8, %rdi

	inc %rbx
	push %rbx
	call __ffly_mem_dup
	pop %rbx

	movq -8(%rbp), %rax
	movq %rbp, %rsp
	pop %rbp
	ret
