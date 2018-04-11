%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __ftruncate
section .text
__ftruncate:
	mov rax, sys_ftruncate
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

