%include "syscall.mac"
section .text
global _ffly_open
_ffly_open:
    mov rax, sys_open
    syscall
    ret
