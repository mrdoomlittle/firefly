%include "syscall.mac"
%include "err.mac"
extern set_errno
global __clone
section .text
__clone:
    mov rax, sys_clone
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault
    ret

	_fault:
	call set_errno
	mov rax, -1
	ret
