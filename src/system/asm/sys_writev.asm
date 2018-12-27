%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __writev
section .text
__writev:
	mov rax, sys_writev
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
