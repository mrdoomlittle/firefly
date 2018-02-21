%include "syscall.mac"
%include "err.mac"
extern set_errno
global __write
section .text
__write:
	mov rax, sys_write
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault:
	call set_errno
	mov rax, -1
	ret
