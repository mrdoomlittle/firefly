%include "syscall.mac"
section .text
global _shmget
_shmget:
	mov rax, sys_shmget
	syscall
	ret
