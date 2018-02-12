%include "syscall.mac"
section .text
global _shmat
_shmat:
	mov rax, sys_shmat
	syscall
	ret
