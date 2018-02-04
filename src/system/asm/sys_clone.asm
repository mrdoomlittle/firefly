%include "syscall.mac"
section .text
global _ffly_clone
_ffly_clone:
    mov rax, sys_clone
    syscall
    ret
