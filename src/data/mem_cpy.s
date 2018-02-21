	.globl __ffly_mem_cpy
	.text
/*
 * rdi - dst
 * rsi - src
 * rbx - count
 */
__ffly_mem_cpy:
	movq %rsi, %rcx
	_next:
	movq %rcx, %rax
	subq %rcx, %rax

	cmp $64, %rax
	jae _mov512
	cmp $32, %rax
	jae _mov256
	cmp $16, %rax
	jae _mov128
	cmp $8, %rax
	jae _mov64
	cmp $4, %rax
	jae _mov32
	cmp $2, %rax
	jae _mov16
	cmp $1, %rax
	jae _mov8
	ret

	_mov512:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq 16(%rsi), %r10
	movq 24(%rsi), %r11
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	movq 16(%rdi), %r10
	movq 24(%rdi), %r11
	addq $32, %rdi
	addq $32, %rsi
	_mov256:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq 16(%rsi), %r10
	movq 24(%rsi), %r11
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	movq 16(%rdi), %r10
	movq 24(%rdi), %r11
	addq $32, %rdi
	addq $32, %rsi
	jmp _sk
	_mov128:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	addq $16, %rdi
	addq $16, %rsi
	jmp _sk
	_mov64:
	movq (%rsi), %r8
	movq %r8, (%rdi)
	addq $8, %rdi
	addq $8, %rsi
	jmp _sk
	_mov32:
	movl (%rsi), %r8d
	movl %r8d, (%rdi)
	addq $4, %rdi
	addq $4, %rsi
	jmp _sk
	_mov16:
	movw (%rsi), %r8w
	movw %r8w, (%rdi)
	addq $2, %rdi
	addq $2, %rsi
	jmp _sk
	_mov8:
	movb (%rsi), %r8b
	movb %r8b, (%rdi)
	addq $1, %rdi
	addq $1, %rsi
	_sk:
	cmp $0, %rax
	ja _next
	ret
