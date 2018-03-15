%include "syscall.mac"
%include "err.mac"
extern set_errno
global __shutdown
section .text
__shutdown:
	mov rax, sys_shutdown
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

