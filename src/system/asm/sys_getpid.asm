%include "syscall.mac"
section .text
global _ffly_getpid
_ffly_getpid:
	mov rax, sys_getpid
	syscall
	ret
