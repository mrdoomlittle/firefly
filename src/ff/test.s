.region text
_start:
rin 0
call $main
exit %ll
.globl ts0
ts0:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 3
subq %sp, %rlx, %sp
asb %al, 2
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
asb %al, 1
asq %rlx, 2
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 2
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
asb %al, 2
asq %rlx, 3
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 3
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
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
asq %rlx, 3
subq %sp, %rlx, %sp
asb %al, 1
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asb %al, 0
asq %rlx, 2
subq %bp, %rlx, %rlx
ldb %rlx, %al
asb %al, 1
asq %rlx, 3
subq %bp, %rlx, %rlx
ldb %rlx, %al
call $ts0
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
asq %rlx, 2
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
asq %rlx, 3
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
