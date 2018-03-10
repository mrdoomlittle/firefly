%include "syscall.mac"
section .text
global __shmat
__shmat:
	mov rax, sys_shmat
	syscall
	ret
