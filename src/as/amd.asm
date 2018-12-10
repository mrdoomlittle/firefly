.region text
_start:
/
push %rax

push %eax
push %ax
push %al

pop %rax
pop %eax
pop %ax
xor %ebx, 0xffffffff
xor %bx, 0xffff
xor %bl, 0xff

xor %ecx, 0xffffffff
xor %cx, 0xffff
xor %cl, 0xff
mov %rbp, 0xffffffffffffffff
mov %rsp, 0xffffffffffffffff

push %rbp
mov %rsp, %rbp



mov %rbp, %rsp
pop %rbp
/


callq $test

movq %rax, 60
syscall
test:
ret
.endof
