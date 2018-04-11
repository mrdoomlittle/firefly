%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __shmget
section .text
__shmget:
	mov rax, sys_shmget
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
