	.globl __ffly_mem_cmp
	.text
/*
*	rdi - p0
*	rsi - p1
*	rbx - no
*/
__ffly_mem_cmp:
	push %rbp
	movq %rsp, %rbp
	sub $2, %rsp
	movw $0, -2(%rbp)
.L0:
	movb (%rdi), %al
	cmpb %al, (%rsi)
	jne .L2
	incw -2(%rbp)
	cmpw %bx, -2(%rbp)
	je .L1
	inc %rdi
	inc %rsi

	jmp .L0
.L1:
	movb $0x0, %al
	jmp .L3
.L2:
	movb $0xff, %al
.L3:
	movq %rbp, %rsp
	pop %rbp
	ret
