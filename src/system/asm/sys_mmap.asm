%include "syscall.mac"
section .text
global _ffly_mmap
_ffly_mmap:
	mov rax, sys_mmap
	syscall
	ret
