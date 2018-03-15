%include "syscall.mac"
%include "err.mac"
extern set_errno
global __connect
section .text
__connect:
	mov rax, sys_connect
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

