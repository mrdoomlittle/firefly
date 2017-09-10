section .text
global ffly_sq
ffly_sq:
	push rbp
	mov rbp,rsp
	sub rsp,8
	movsd [rbp-8],xmm0

	fld qword[rbp-8]
	fmul ST0

	fstp qword[rbp-8]
	movsd xmm0,[rbp-8]

	mov rsp,rbp
	pop rbp
	ret
