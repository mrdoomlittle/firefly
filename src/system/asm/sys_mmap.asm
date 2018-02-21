%include "syscall.mac"
%include "err.mac"
extern set_errno
global __mmap
section .text
__mmap:
	mov rax, sys_mmap
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
	_fault:
	call set_errno
	mov rax, -1
	ret
