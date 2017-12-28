%include "errno.mac"
section .text
global _ffly_mutex_lock
global _ffly_mutex_unlock
global _ffly_mutex_trylock
_ffly_mutex_lock:
	mov al, [rdi]
	cmp al, 1
	je _ffly_mutex_lock

	mov al, 1
	lock xchg [rdi], al
	cmp al, 0
	jne _ffly_mutex_lock
	ret
_ffly_mutex_unlock:
	mov al, [rdi]
	lock xor BYTE[rdi], al    
	ret
_ffly_mutex_trylock:
	mov al, 1
	lock xchg [rdi], al
	cmp al, 0
	je _success
	mov bl, FFLY_FAILURE
	ret
	_success:
	mov bl, FFLY_SUCCESS
	ret
