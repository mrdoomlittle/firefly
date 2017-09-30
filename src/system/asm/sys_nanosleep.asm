section .text
global ffly_usleep
ffly_usleep:
	push rbp
	mov rbp, rsp
	sub rsp, 12

	mov rbx, rsi
	mov [rbp-12], rdi
	mov [rbp-4], rbx

	mov rax, 35
	mov rdi, rbp
	sub rdi, 12
	xor rsi, rsi
	syscall

	mov rsp, rbp
	pop rbp
	ret
