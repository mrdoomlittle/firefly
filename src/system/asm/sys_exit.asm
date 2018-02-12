%include "syscall.mac"
section .text
global _exit
_exit:
	mov rax, sys_exit
	syscall
	ret
