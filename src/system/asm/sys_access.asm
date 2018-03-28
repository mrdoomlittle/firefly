%include "syscall.mac"
%include "err.mac"
extern set_errno
global __access
section .text
__access:
	mov rax, sys_access
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault:
	call set_errno
	mov rax, -1
	ret
