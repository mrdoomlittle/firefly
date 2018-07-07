%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __timer_settime
section .text
__timer_settime:
    mov rax, sys_timer_settime
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault
    ret

_fault:
	call __set_errno
	mov rax, -1
	ret
