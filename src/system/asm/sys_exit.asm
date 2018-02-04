%include "syscall.mac"
section .text
global _ffly_exit
_ffly_exit:
	mov rax, sys_exit
	syscall
	ret
