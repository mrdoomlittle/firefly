%include "syscall.mac"
global __ffly_nanosleep
section .text
__ffly_nanosleep:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	push rax

	mov [rbp-16], rdi
	mov [rbp-8], rsi

	mov rax, sys_nanosleep
	mov rdi, rbp
	sub rdi, 16
	xor rsi, rsi
	syscall

	pop rax

	mov rsp, rbp
	pop rbp
	ret
