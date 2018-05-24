global __ffly_cond_lock_wait
global __ffly_cond_lock_signal
extern __ffly_nanosleep
section .text
__ffly_cond_lock_wait:
	push rbp
	mov rbp, rsp
	sub rsp, 8

	push rsi
	push rax

	mov QWORD[rbp-8], rdi

	jmp _sk_sleep

_re_check:
	mov rdi, 0

	mov rsi, 1000
	call __ffly_nanosleep

_sk_sleep:
	mov rdi, QWORD[rbp-8]
	mov al, BYTE[rdi]
	and al, 0x1
	cmp al, 0x1
	jne _re_check
	xor BYTE[rdi], 0x1

	pop rax
	pop rsi

	mov rsp, rbp
	pop rbp
	ret
__ffly_cond_lock_signal:
	or BYTE[rdi], 0x1
	ret
