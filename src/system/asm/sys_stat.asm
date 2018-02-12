%include "syscall.mac"
section .text
global _stat
_stat:
	mov rax, sys_stat
	syscall
	ret
