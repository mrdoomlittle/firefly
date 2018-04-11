%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __accept
section .text
__accept:
	mov rax, sys_accept
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

