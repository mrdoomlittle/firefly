%include "syscall.mac"
section .text
global _ffly_getppid
_ffly_getppid:
	mov rax, sys_getppid
	syscall
	ret
