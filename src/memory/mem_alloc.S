	.globl __ffly_mal
	.globl ffly_mal
	.text
// used by mem_dup
__ffly_mal:
	pushq %rbp
	movq %rsp, %rbp
	movq 16(%rbp), %rax
#ifdef __ffly_mal_track
	subq $1, %rsp
	movb $0x0, (%rsp)
#endif
#if (!defined(__ff32) && !defined(__ff64)) || defined(__ff32)
	subq $4, %rsp
	movl %eax, (%rsp)
#elif defined(__ff64)
	subq $8, %rsp
	movq %rax, (%rsp)
#else
#error "error"
#endif
	call ffly_mal
	movq %rbp, %rsp
	popq %rbp
	ret
