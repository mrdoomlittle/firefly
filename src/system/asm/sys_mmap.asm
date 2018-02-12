%include "syscall.mac"
section .text
global _mmap
_mmap:
	mov rax, sys_mmap
	syscall
	ret
