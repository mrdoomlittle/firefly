%include "syscall.mac"
%include "err.mac"
extern set_errno
global __execve
section .text
__execve:
    mov rax, sys_execve
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault
    ret

	_fault:
	call set_errno
	mov rax, -1
	ret
