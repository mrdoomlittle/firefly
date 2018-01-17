section .text
global _ffly_brk
_ffly_brk:
    mov rax, 12
    syscall
    ret
