%include "syscall.mac"
section .text
global _shmdt
_shmdt:
    mov rax, sys_shmdt
    syscall
    ret
