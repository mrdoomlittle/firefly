%include "syscall.mac"
section .text
global __shmdt
__shmdt:
    mov rax, sys_shmdt
    syscall
    ret
