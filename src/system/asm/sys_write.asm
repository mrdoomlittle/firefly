section .text
global _ffly_write
_ffly_write:
    mov rax, 1
    syscall
    ret
