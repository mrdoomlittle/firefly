%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __mkdir
section .text
__mkdir:
	mov rax, sys_mkdir
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

