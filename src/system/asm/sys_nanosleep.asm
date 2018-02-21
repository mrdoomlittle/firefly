%include "syscall.mac"
global _ffly_nanosleep
section .text
_ffly_nanosleep:
	push rbp
	mov rbp, rsp
	sub rsp, 16

	mov [rbp-16], rdi
	mov [rbp-8], rsi

	mov rax, sys_nanosleep
	mov rdi, rbp
	sub rdi, 16
	xor rsi, rsi
	syscall

	mov rsp, rbp
	pop rbp
	ret
