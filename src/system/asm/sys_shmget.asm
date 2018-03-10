%include "syscall.mac"
section .text
global __shmget
__shmget:
	mov rax, sys_shmget
	syscall
	ret
