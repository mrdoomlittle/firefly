%include "syscall.mac"
section .text
global _ffly_stat
_ffly_stat:
	mov rax, sys_stat
	syscall
	ret
