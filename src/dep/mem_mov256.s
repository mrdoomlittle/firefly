.globl __ffly_mem_mov256
__ffly_mem_mov256:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq 16(%rsi), %r10
	movq 24(%rsi), %r11
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	movq 16(%rdi), %r10
	movq 24(%rdi), %r11
    ret
