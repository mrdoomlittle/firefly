%include "syscall.mac"
section .text
global _ffly_write
_ffly_write:
	mov rax, sys_write
	syscall
	ret
