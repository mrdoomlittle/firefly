%include "syscall.mac"
%include "err.mac"
extern set_errno
global __getsockopt
section .text
__getsockopt:
	mov rax, sys_getsockopt
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

