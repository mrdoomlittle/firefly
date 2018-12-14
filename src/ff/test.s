	.region text
main:
pushq %rbp
movq %rbp, %rsp
movq %rsp, %rbp
popq %rbp
ret
_start:
pushq %rbp
movq %rbp, %rsp
callq $main
movq %rax, 60
syscall
movq %rsp, %rbp
popq %rbp
ret
.endof
