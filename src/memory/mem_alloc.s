	.globl __ffly_mal
	.text
__ffly_mal:
	push %rbp
	movq %rsp, %rbp
# ifdef __ffly_mal_track
	sub $1, %rsp
# endif
# if (!defined(__arc32) && !defined(__arc64)) || defined(__arc32)
	sub $4, %rsp
# else
# ifdef __arc64
	sub $8, %rsp
# endif
# endif


	movq %rbp, %rsp
	pop %rbp
	ret
