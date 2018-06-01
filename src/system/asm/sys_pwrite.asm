%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __pwrite
section .text
__pwrite:
	mov rax, sys_pwrite64
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
