%include "syscall.mac"
section .text
global _getcwd
_getcwd:
	mov rax, sys_getcwd
	syscall
	ret
