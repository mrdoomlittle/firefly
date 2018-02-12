%include "syscall.mac"
section .text
global _fsync
_fsync:
	mov rax, sys_fsync
	syscall
	ret
