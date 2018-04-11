%include "syscall.mac"
%include "err.mac"
extern __set_errno
global _getcwd
section .text
_getcwd:
	mov rax, sys_getcwd
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
