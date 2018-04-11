%include "syscall.mac"
%include "err.mac"
extern __set_errno
global __lseek
section .text
__lseek:
    mov rax, sys_lseek
    syscall
	cmp rax, -MAX_ERRNO
	jae _fault

    ret
_fault:
	call __set_errno
	mov rax, -1
	ret
