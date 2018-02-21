%include "syscall.mac"
%include "err.mac"
extern set_errno
global _brk
section .text
_brk:
	mov rax, sys_brk
	syscall
	mov rbx, rax
	cmp rbx, -MAX_ERRNO
	jae _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

