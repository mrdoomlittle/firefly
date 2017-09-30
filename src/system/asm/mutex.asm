section .text
global ffly_mutex_lock
global ffly_mutex_unlock
ffly_mutex_lock:
; check until not locked
	_re_check:
	mov al, [rdi]
	cmp al, 1
	je _re_check

; incrment to a locked value
	lock inc BYTE[rdi]
; check if correct else try again to lock
	cmp BYTE[rdi], 1
	jne ffly_mutex_lock
	ret
ffly_mutex_unlock:
	mov al, [rdi]
; check if locked, if not skip the decrementation instruction
	cmp al, 1
	jne _sk_decr
	lock dec BYTE[rdi]
	_sk_decr:
	ret
