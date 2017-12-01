section .text
global ffly_sigwait
global ffly_signal
extern ffly_nanosleep
ffly_sigwait:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	mov QWORD[rbp-8], rdi
	jmp _sk_sleep
	_again:
	mov rdi, 0
	mov rsi, 1000
	call ffly_nanosleep
	_sk_sleep:
	mov rdi, QWORD[rbp-8]
	mov ax, WORD[rdi]
	and ax, 0x0001
	cmp ax, 0x0001
	jne _again

	lock xor WORD[rdi], 0x0001
	mov rsp, rbp
	pop rbp
ffly_signal:
	_re_check:
	mov ax, WORD[rdi]
	and ax, 0x0001
	cmp ax, 0x0001
	je _re_check
	lock or WORD[rdi], 0x0001
