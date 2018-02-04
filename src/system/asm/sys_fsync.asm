%include "syscall.mac"
section .text
global _ffly_fsync
_ffly_fsync:
	mov rax, sys_fsync
	syscall
	ret
