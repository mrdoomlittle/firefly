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
.L0:
	movb (%rsi), %al
	movb %al, (%rdi)
	inc %rdi
	inc %rsi
	dec %rbx
	cmpq $0, %rbx
	ja .L0
	pop %rax
	mov %rbp, %rsp
	pop %rbp
	ret
