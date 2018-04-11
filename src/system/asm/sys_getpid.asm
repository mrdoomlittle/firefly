%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __getpid
section .text
__getpid:
	mov rax, sys_getpid
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
