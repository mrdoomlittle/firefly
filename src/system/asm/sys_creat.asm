%include "syscall.mac"
section .text
global _creat
_creat:
    mov rax, sys_creat
    syscall
    ret
