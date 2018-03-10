%include "syscall.mac"
%include "err.mac"
extern set_errno
global __wait4
section .text
__wait4:
    mov rax, sys_wait4
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault
    ret

	_fault:
	call set_errno
	mov rax, -1
	ret
