%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __creat
section .text
__creat:
    mov rax, sys_creat
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault

    ret
_fault:
	call __set_errno
	mov rax, -1
	ret
