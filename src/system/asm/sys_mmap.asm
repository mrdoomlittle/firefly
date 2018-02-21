%include "syscall.mac"
%include "err.mac"
extern set_errno
global _mmap
section .text
_mmap:
	mov rax, sys_mmap
	syscall
	mov rbx, rax
	cmp rbx, -MAX_ERRNO
	jae _fault

	ret
	_fault:
	call set_errno
	mov rax, -1
	ret
