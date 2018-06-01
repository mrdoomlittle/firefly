%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __pread
section .text
__pread:
	mov rax, sys_pread64
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault:
	call __set_errno
	mov rax, -1
	ret
