%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __setsockopt
section .text
__setsockopt:
	mov rax, sys_setsockopt
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

