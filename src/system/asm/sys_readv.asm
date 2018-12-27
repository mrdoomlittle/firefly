%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __readv
section .text
__readv:
	mov rax, sys_readv
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
