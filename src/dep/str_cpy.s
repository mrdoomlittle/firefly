	.globl __ffly_str_cpy
	.globl __ffly_str_len
	.globl __ffly_mem_cpy
	.text
/*
*	rdi - dst
*	rsi - src
*/
__ffly_str_cpy:
	push %rbx
	push %rcx

	push %rdi
	push %rsi

	movq %rsi, %rdi
	call __ffly_str_len
	pop %rsi
	pop %rdi

	push %rax

	movq %rdi, %rcx
	add %rax, %rcx
	inc %rbx
	movb $0x0, (%rcx)

	movq %rax, %rbx
	call __ffly_mem_cpy
	pop %rax
	pop %rcx
	pop %rbx
	ret
