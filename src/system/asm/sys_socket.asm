%include "syscall.mac"
%include "err.mac"
extern set_errno
global __socket
section .text
__socket:
	mov rax, sys_socket
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

