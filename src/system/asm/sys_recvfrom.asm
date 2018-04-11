%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __recvfrom
section .text
__recvfrom:
	mov rax, sys_recvfrom
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

