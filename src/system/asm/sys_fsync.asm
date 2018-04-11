%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __fsync
section .text
__fsync:
	mov rax, sys_fsync
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
