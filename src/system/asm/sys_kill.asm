%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __kill
section .text
__kill:
	mov rax, sys_kill
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
