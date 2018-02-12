%include "syscall.mac"
section .text
global _lseek
_lseek:
    mov rax, sys_lseek
    syscall
    ret
