%include "syscall.mac"
section .text
global _open
_open:
    mov rax, sys_open
    syscall
    ret
