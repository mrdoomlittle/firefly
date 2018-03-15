%include "syscall.mac"
%include "err.mac"
extern set_errno
global __listen
section .text
__listen:
	mov rax, sys_listen
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

