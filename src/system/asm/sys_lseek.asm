%include "syscall.mac"
section .text
global _ffly_lseek
_ffly_lseek:
    mov rax, sys_lseek
    syscall
    ret
