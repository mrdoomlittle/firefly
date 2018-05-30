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
.L0:
	mov (%rdi), %al
	cmpb %al, (%rsi)
	jne .L1

	incw -2(%rbp)

	cmpb $0x0, (%rdi)
	je .L1
	cmpb $0x0, (%rsi) 
	je .L1
	inc %rdi
	inc %rsi
	jmp .L0
.L1:

	decw -2(%rbp)

.L2:
	inc %rdi
	cmpb $0x0, -1(%rdi)
	jne .L2
	dec %rdi

.L3:
	inc %rsi
	cmpb $0x0, -1(%rsi)
	jne .L3
	dec %rsi

	sub -10(%rbp), %rdi
	sub -18(%rbp), %rsi
	cmpq %rdi, %rsi
	jne .L4

	xorq %rax, %rax
	movw -2(%rbp), %ax
	cmp %rdi, %rax
	jne .L4
	mov $0x0, %al
	jmp .L5
.L4:
	mov $0xff, %al
.L5:
	movq %rbp, %rsp
	pop %rbp
	ret
