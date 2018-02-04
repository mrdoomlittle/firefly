%include "syscall.mac"
section .text
global _ffly_close
_ffly_close:
    mov rax, sys_close
    syscall
    ret
