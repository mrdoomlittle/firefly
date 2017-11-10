section .text
global ffly_mutex_lock
global ffly_mutex_unlock
ffly_mutex_lock:
	mov al, [rdi]
	cmp al, 1
	je ffly_mutex_lock

	lock xchg [rdi], al
	cmp al, 0
	jne ffly_mutex_lock
	ret
ffly_mutex_unlock:
	mov dl, [rdi]
	lock xor BYTE[rdi], al
	_sk_decr:
	ret
