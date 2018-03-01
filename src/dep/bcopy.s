	.globl __ffly_bcopy
	.text
/*
*	rdi - dst 
*	rsi - src
*	rbx - no
*/
__ffly_bcopy:
	_again:
	movb (%rsi), %al
	movb %al, (%rdi)
	inc %rdi
	inc %rsi
	dec %rbx
	cmpq $0, %rbx
	ja _again
	ret
