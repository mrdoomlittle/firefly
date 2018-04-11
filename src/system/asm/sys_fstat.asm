%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __fstat
section .text
__fstat:
	mov rax, sys_fstat
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
