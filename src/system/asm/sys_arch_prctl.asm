%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __arch_prctl
section .text
__arch_prctl:
	mov rax, sys_arch_prctl
	syscall
	cmp rax, -MAX_ERRNO
	jae _fault

	ret
_fault: 
	call __set_errno
	mov rax, -1
	ret

