%include "syscall.mac"
section .text
global _access
_access:
	mov rax, sys_access
	syscall
	ret
