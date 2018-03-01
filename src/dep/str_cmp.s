	.globl __ffly_str_cmp
	.text
/*
*	rdi - s0
*	rsi - s1
*/
__ffly_str_cmp:
	push %rbp
	movq %rsp, %rbp
	sub $18, %rsp

	movq %rdi, -10(%rbp)
	movq %rsi, -18(%rbp)
	movw $0, -2(%rbp)
	_again:
	mov (%rdi), %al
	cmpb %al, (%rsi)
	jne _end

	incw -2(%rbp)

	cmpb $0x0, (%rdi)
	je _end
	cmpb $0x0, (%rsi) 
	je _end
	inc %rdi
	inc %rsi
	jmp _again
	_end:

	decw -2(%rbp)

	_n0:
	inc %rdi
	cmpb $0x0, -1(%rdi)
	jne _n0
	dec %rdi

	_n1:
	inc %rsi
	cmpb $0x0, -1(%rsi)
	jne _n1
	dec %rsi

	sub -10(%rbp), %rdi
	sub -18(%rbp), %rsi
	cmpq %rdi, %rsi
	jne _j0

	xorq %rax, %rax
	movw -2(%rbp), %ax
	cmp %rdi, %rax
	jne _j0
	mov $0x0, %al
	jmp _sk
	_j0:
	mov $0xff, %al
	_sk:
	movq %rbp, %rsp
	pop %rbp
	ret
