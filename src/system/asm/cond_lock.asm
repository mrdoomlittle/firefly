section .text
global ffly_cond_lock_wait
global ffly_cond_lock_signal
ffly_cond_lock_wait:
	_re_check:
	mov al, [rdi]
	and al, 1
	cmp al, 0
	je _re_check
	mov BYTE[rdi], 0
	ret
ffly_cond_lock_signal:
	mov BYTE[rdi], 1
	ret
