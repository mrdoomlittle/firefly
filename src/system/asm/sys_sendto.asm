%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __sendto
section .text
__sendto:
	mov rax, sys_sendto
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

