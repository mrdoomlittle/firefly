%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __clock_gettime
section .text
__clock_gettime:
	mov rax, sys_clock_gettime
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

