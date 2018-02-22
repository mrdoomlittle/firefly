	.globl __ffly_str_len
	.text
/*
*	rdi - string
*	rax - count
*/
__ffly_str_len:
	xorq %rax, %rax
	movq %rdi, %rbx

	_again:
	incq %rdi
	cmpb $0x0, -1(%rdi)
	jne _again
	decq %rdi

	subq %rbx, %rdi
	movq %rdi, %rax
	ret
