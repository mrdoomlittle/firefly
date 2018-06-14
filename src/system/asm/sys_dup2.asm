%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __dup2
section .text
__dup2:
    mov rax, sys_dup2
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault

    ret
_fault:
	call __set_errno
	mov rax, -1
	ret
