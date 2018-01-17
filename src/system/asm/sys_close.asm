section .text
global _ffly_close
_ffly_close:
    mov rax, 3
    syscall
    ret
