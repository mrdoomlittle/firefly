%include "syscall.mac"
section .text
global _read
_read:
	mov rax, sys_read
	syscall
	ret
