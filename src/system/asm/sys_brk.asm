%include "syscall.mac"
section .text
global _ffly_brk
_ffly_brk:
    mov rax, sys_brk
    syscall
    ret
