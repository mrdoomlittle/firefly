%include "syscall.mac"
section .text
global _ffly_creat
_ffly_creat:
    mov rax, sys_creat
    syscall
    ret
