section .text
global _ffly_nanosleep
_ffly_nanosleep:
	push rbp
	mov rbp, rsp
	sub rsp, 16

	mov [rbp-16], rdi
	mov [rbp-8], rsi

	mov rax, 35
	mov rdi, rbp
	sub rdi, 16
	xor rsi, rsi
	syscall

	mov rsp, rbp
	pop rbp
	ret
