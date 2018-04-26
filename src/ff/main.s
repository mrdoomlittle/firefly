.region text
.globl ts0
ts0:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 0
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
call $ts1
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts1
ts1:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 1
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
call $ts2
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts2
ts2:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 2
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
call $ts3
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts3
ts3:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 3
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
call $ts4
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts4
ts4:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 4
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
call $ts5
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ts5
ts5:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 5
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
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
asq %rlx, 1
subq %sp, %rlx, %sp
asb %al, 21
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %bp, %rlx, %rlx
stb %rlx, %al
outb %al
call $ts0
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
