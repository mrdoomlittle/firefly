%include "syscall.mac"
section .text
global _fork
_fork:
	mov rax, sys_fork
	syscall
	ret
