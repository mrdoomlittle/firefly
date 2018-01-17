section .text
global _ffly_open
_ffly_open:
    mov rax, 2
    syscall
    ret
