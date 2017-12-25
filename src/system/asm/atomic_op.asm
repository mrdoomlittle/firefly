section .text
global _ffly_atomic_add
global _ffly_atomic_sub
global _ffly_atomic_set
global _ffly_atomic_get
global _ffly_atomic_incr
global _ffly_atomic_decr
_ffly_atomic_add:
	lock add [rdi], rsi
	ret
_ffly_atomic_sub:
	lock sub [rdi], rsi
	ret
_ffly_atomic_set:
	mov [rdi], rsi
	ret
_ffly_atomic_get:
	mov rax, [rdi]
	ret
_ffly_atomic_incr:
	lock inc QWORD[rdi]
	ret
_ffly_atomic_decr:
	lock dec QWORD[rdi]
	ret
