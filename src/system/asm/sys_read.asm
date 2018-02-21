%include "syscall.mac"
%include "err.mac"
extern set_errno
global __read
section .text
__read:
	mov rax, sys_read
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault:
	call set_errno
	mov rax, -1
	ret
