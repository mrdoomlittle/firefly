section .text
global _fcntl
_fcntl:
    mov rax, 72
    syscall
    ret
