%include "syscall.mac"
section .text
global _fstat
_fstat:
	mov rax, sys_fstat
	syscall
	ret
