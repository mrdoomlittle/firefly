	.globl __ffly_mal
	.globl ffly_mal
	.text
// used by mem_dup
__ffly_mal:
	push %rbp
	movq %rsp, %rbp
	movq 16(%rbp), %rax
#ifdef __ffly_mal_track
	sub $1, %rsp
	movb $0x0, (%rsp)
#endif
#if (!defined(__ff32) && !defined(__ff64)) || defined(__ff32)
	sub $4, %rsp
	movl %eax, (%rsp)
#else
#ifdef __ff64
	sub $8, %rsp
	movq %rax, (%rsp)
#endif
#endif
	call ffly_mal
	movq %rbp, %rsp
	pop %rbp
	ret
