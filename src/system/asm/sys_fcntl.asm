section .text
global _ffly_fcntl
_ffly_fcntl:
    mov rax, 72
    syscall
    ret
