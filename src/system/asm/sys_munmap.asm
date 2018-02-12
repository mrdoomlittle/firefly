%include "syscall.mac"
section .text
global _munmap
_munmap:
	mov rax, sys_munmap
	syscall
	ret
