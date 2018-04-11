%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __rmdir
section .text
__rmdir:
    mov rax, sys_rmdir
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault
    ret

_fault:
	call __set_errno
	mov rax, -1
	ret
