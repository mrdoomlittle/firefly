%include "syscall.mac"
section .text
global _close
_close:
    mov rax, sys_close
    syscall
    ret
