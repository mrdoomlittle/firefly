%include "errno.mac"
global __ffly_mutex_lock
global __ffly_mutex_unlock
global __ffly_mutex_trylock
section .text
__ffly_mutex_lock:
	mov al, [rdi]
	cmp al, 1
	je __ffly_mutex_lock

	mov al, 1
	lock xchg [rdi], al
	cmp al, 0
	jne __ffly_mutex_lock
	ret
__ffly_mutex_unlock:
	mov al, [rdi]
	lock xor BYTE[rdi], al    
	ret
__ffly_mutex_trylock:
	mov al, 1
	lock xchg [rdi], al
	cmp al, 0
	je _succ
	mov bl, FFLY_FAILURE
	ret
_succ:
	mov bl, FFLY_SUCCESS
	ret
