section .text
global ffly_sys_write
; for later
ffly_sys_write:
	push rbp
	mov rbp,rsp
	sub rsp,16
	mov [rbp-16], rax
	mov [rbp-8], rdi

	mov rax, 1
	mov rdx, [rbp-8]
	mov rsi, [rbp-16]
	mov rdi, 1
	syscall

	mov rsp, rbp
	pop rbp
	ret
