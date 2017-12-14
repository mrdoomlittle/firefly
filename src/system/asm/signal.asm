section .text
global ffly_sigwait
global ffly_signal
global ffly_try_signal
extern ffly_nanosleep
ffly_sigwait:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	mov QWORD[rbp-8], rdi
	lock or WORD[rdi], 0x0002
	jmp _sk_sleep
	_again:
	mov rdi, 0
	mov rsi, 1000
	call ffly_nanosleep
	_sk_sleep:
	mov rdi, QWORD[rbp-8]
	mov ax, WORD[rdi]
	and ax, 0x0001
	cmp ax, 0x0000
	je _again

	lock xor WORD[rdi], 0x0001
	lock xor WORD[rdi], 0x0002
	mov rsp, rbp
	pop rbp
	ret
ffly_signal:
	mov ax, WORD[rdi]
	and ax, 0x0002
	cmp ax, 0x0002
	jne _end

	_re_check:
	mov ax, WORD[rdi]
	and ax, 0x0001
	cmp ax, 0x0001
	je _re_check
	lock or WORD[rdi], 0x0001
	_end:
	ret
