%include "syscall.mac"
section .text
global _kill
_kill:
	mov rax, sys_kill
	syscall
	ret

