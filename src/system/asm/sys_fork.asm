%include "syscall.mac"
section .text
global __fork
__fork:
	mov rax, sys_fork
	syscall
	ret
