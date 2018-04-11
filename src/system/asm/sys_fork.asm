%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __fork
section .text
__fork:
	mov rax, sys_fork
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
