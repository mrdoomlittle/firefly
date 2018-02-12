%include "syscall.mac"
section .text
global _brk
_brk:
	mov rax, sys_brk
	syscall
	ret
