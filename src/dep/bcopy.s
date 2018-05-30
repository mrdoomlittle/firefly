	.globl __ffly_bcopy
	.text
/*
*	rdi - dst 
*	rsi - src
*	rbx - no
*/
__ffly_bcopy:
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
	ret
