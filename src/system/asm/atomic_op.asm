global __ffly_atomic_add
global __ffly_atomic_sub
global __ffly_atomic_set
global __ffly_atomic_get
global __ffly_atomic_incr
global __ffly_atomic_decr
section .text
__ffly_atomic_add:
	lock add [rdi], rsi
	ret
__ffly_atomic_sub:
	lock sub [rdi], rsi
	ret
__ffly_atomic_set:
	mov [rdi], rsi
	ret
__ffly_atomic_get:
	mov rax, [rdi]
	ret
__ffly_atomic_incr:
	lock inc QWORD[rdi]
	ret
__ffly_atomic_decr:
	lock dec QWORD[rdi]
	ret
