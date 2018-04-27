.region text
.globl abort
abort:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 8
subq %sp, %rlx, %sp
asq %rax, 0
asq %rlx, 8
subq %bp, %rlx, %rlx
stb %rlx, %al
exit %al
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts0
ts0:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 32
subq %sp, %rlx, %sp
asq %rax, 0
asq %rlx, 8
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
asq %rax, 0
asq %rlx, 16
subq %bp, %rlx, %rlx
stw %rlx, %ax
outw %ax
asq %rax, 0
asq %rlx, 24
subq %bp, %rlx, %rlx
std %rlx, %eax
outd %eax
asq %rax, 0
asq %rlx, 32
subq %bp, %rlx, %rlx
stq %rlx, %rax
outq %rax
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl main
main:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asq %rax, 0
asb %al, 21
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rax, 0
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
asq %rlx, 17
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rax, 0
asb %al, 4
asq %rlx, 25
subq %bp, %rlx, %rlx
ldw %rlx, %ax
asq %rax, 0
asb %al, 6
asq %rlx, 33
subq %bp, %rlx, %rlx
ldd %rlx, %eax
asq %rax, 0
asb %al, 8
asq %rlx, 41
subq %bp, %rlx, %rlx
ldq %rlx, %rax
call $ts0
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
