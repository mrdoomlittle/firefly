.globl __ffly_mem_mov128
__ffly_mem_mov128:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	ret
