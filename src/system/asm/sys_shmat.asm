%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __shmat
section .text
__shmat:
	mov rax, sys_shmat
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
