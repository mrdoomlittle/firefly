%include "syscall.mac"
section .text
global _clone
_clone:
    mov rax, sys_clone
    syscall
    ret
