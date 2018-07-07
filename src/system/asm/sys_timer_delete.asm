%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __timer_delete
section .text
__timer_delete:
    mov rax, sys_timer_delete
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault
    ret

_fault:
	call __set_errno
	mov rax, -1
	ret
