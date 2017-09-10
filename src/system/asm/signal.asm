section .text
global ffly_sigwait
global ffly_signal
extern usleep
ffly_sigwait:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	mov QWORD[rbp-8], rdi
	jmp _sk_wait
	_again:
	mov rdi, 1000
	call usleep
	_sk_wait:
	mov rdi, [rbp-8]
	mov ax, WORD[rdi]
	and ax, 1
	cmp ax, 1
	jne _again

	mov rdi, [rbp-8]
	lock xor WORD[rdi], 1
	mov rsp, rbp
	pop rbp
ffly_signal:
	_re_check:
	mov ax, [rdi]
	and ax, 1
	cmp ax, 1
	je _re_check
	lock or WORD[rdi], 1
