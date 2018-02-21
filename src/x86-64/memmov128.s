.globl __ffly_memmov128
__ffly_memmov128:
	movq (%rsi), %r8
	movq 8(%rsi), %r9
	movq (%rdi), %r8
	movq 8(%rdi), %r9
	ret
