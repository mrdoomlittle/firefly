	.globl __ffly_str_len
	.text
/*
*	rdi - string
*	rax - count
*/
__ffly_str_len:
	xorq %rax, %rax
	push %rbx
	movq %rdi, %rbx
.L0:
	inc %rdi
	cmpb $0x0, -1(%rdi)
	jne .L0
	dec %rdi

	sub %rbx, %rdi
	movq %rdi, %rax
	pop %rbx
	ret
