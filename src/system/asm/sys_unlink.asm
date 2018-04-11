%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __unlink
section .text
__unlink:
	mov rax, sys_unlink
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

