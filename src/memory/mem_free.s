	.globl __ffly_mfr
	.text
__ffly_mfr:
	push %rbp
	movq %rsp, %rbp



	call ffly_mem_alloc
	movq %rbp, %rsp
	pop %rbp
	ret
