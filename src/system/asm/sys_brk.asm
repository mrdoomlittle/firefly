%include "syscall.mac"
extern set_errno
global _brk
section .text
_brk:
	mov rax, sys_brk
	syscall
	mov rbx, rax
	and rbx, 0xffffffffffffff00
	cmp rbx, 0xffffffffffffff00
	je _fault

	ret
	_fault: 
	call set_errno
	mov rax, -1
	ret

