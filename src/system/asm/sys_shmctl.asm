%include "syscall.mac"
section .text
global __shmctl
__shmctl:
	mov rax, sys_shmctl
	syscall
	ret
