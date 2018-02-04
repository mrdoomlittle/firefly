%include "syscall.mac"
section .text
global _ffly_fork
_ffly_fork:
	mov rax, sys_fork
	syscall
	ret
