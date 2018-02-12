%include "syscall.mac"
section .text
global _getppid
_getppid:
	mov rax, sys_getppid
	syscall
	ret
