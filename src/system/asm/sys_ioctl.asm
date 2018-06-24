%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __ioctl
section .text
__ioctl:
    mov rax, sys_ioctl
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault

    ret
_fault:
	call __set_errno
	mov rax, -1
	ret
