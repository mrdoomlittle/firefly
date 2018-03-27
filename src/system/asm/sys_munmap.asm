%include "syscall.mac"
section .text
global __munmap
__munmap:
	mov rax, sys_munmap
	syscall
	ret
