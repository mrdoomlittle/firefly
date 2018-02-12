%include "syscall.mac"
section .text
global _write
_write:
	mov rax, sys_write
	syscall
	ret
