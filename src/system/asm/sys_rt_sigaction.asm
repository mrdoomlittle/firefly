%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __rt_sigaction
section .text
__rt_sigaction:
	mov rax, sys_rt_sigaction
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

