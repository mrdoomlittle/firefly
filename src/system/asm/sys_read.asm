%include "syscall.mac"
section .text
global _ffly_read
_ffly_read:
	mov rax, sys_read
	syscall
	ret
