%include "syscall.mac"
%include "err.mac"
extern set_errno
global __bind
section .text
__bind:
	mov rax, sys_bind
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret
