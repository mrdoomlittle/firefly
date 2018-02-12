%include "syscall.mac"
section .text
global _shmctl
_shmctl:
	mov rax, sys_shmctl
	syscall
	ret
