.globl __ffly_mem_mov512
__ffly_mem_mov512:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq 16(%rsi), %r10
	movq 24(%rsi), %r11
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	movq 16(%rdi), %r10
	movq 24(%rdi), %r11

	
	movq 32(%rsi), %r8
	movq 40(%rsi), %r9
	movq 48(%rsi), %r10
	movq 56(%rsi), %r11
	movq 32(%rdi), %r8
	movq 40(%rdi), %r9
	movq 48(%rdi), %r10
	movq 56(%rdi), %r11
	ret
