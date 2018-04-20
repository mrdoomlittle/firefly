.region text
.globl ts0
ts0:
outb %ll
call $ts1
ret
.globl ts1
ts1:
outb %ll
call $ts2
ret
.globl ts2
ts2:
outb %ll
call $ts3
ret
.globl ts3
ts3:
outb %ll
ret
.globl main
main:
asb %al, 212
movq %sp, %rlx
stb %rlx, %al
movq %sp, %rlx
ldb %rlx, %al
outb %al
asb %ll, 21
call $ts0
ret
.endof
