%include "syscall.mac"
section .text
global _getpid
_getpid:
	mov rax, sys_getpid
	syscall
	ret
