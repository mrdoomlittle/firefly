%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __shmctl
section .text
__shmctl:
	mov rax, sys_shmctl
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
