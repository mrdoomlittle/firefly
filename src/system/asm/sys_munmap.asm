%include "syscall.mac"
section .text
global _ffly_munmap
_ffly_munmap:
	mov rax, sys_munmap
	syscall
	ret
