%include "syscall.mac"
section .text
global _ffly_access
_ffly_access:
	mov rax, sys_access
	syscall
	ret
