# 1 "memory/mem_alloc.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "memory/mem_alloc.S"
 .globl __ffly_mal
 .globl ffly_mal
 .text

__ffly_mal:
 push %rbp
 movq %rsp, %rbp
 movq 16(%rbp), %rax





 sub $4, %rsp
 movl %eax, (%rsp)






 call ffly_mal
 movq %rbp, %rsp
 pop %rbp
 ret
