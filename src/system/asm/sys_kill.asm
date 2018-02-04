%include "syscall.mac"
section .text
global _ffly_kill
_ffly_kill:
	mov rax, sys_kill
	syscall
	ret

