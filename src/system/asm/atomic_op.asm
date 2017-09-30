section .text
global ffly_atomic_add
global ffly_atomic_sub
global ffly_atomic_set
global ffly_atomic_get
global ffly_atomic_incr
global ffly_atomic_decr
ffly_atomic_add:
	lock add [rdi], rsi
	ret
ffly_atomic_sub:
	lock sub [rdi], rsi
	ret
ffly_atomic_set:
	mov [rdi], rsi
	ret
ffly_atomic_get:
	mov rax, [rdi]
	ret
ffly_atomic_incr:
	lock inc QWORD[rdi]
	ret
ffly_atomic_decr:
	lock dec QWORD[rdi]
	ret
