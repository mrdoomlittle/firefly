%include "syscall.mac"
%include "err.mac"
extern __set_errno
global _getppid
section .text
_getppid:
	mov rax, sys_getppid
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
