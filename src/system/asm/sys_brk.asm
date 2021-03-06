%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __brk
section .text
__brk:
	mov rax, sys_brk
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

