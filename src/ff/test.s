.region text
.globl _exit
_exit:
asb %al, 0
exit %al
.globl _start
_start:
rin 0
asb %al, 21
asq %rlx, 1
subq %sp, %rlx, %rlx
ldb %rlx, %al
asb %al, 31
asq %rlx, 2
subq %sp, %rlx, %rlx
ldb %rlx, %al
asb %al, 41
asq %rlx, 3
subq %sp, %rlx, %rlx
ldb %rlx, %al
asq %rlx, 1
subq %sp, %rlx, %rlx
stb %rlx, %al
outb %al
asq %rlx, 2
subq %sp, %rlx, %rlx
stb %rlx, %al
outb %al
asq %rlx, 3
subq %sp, %rlx, %rlx
stb %rlx, %al
outb %al
asq %rlx, 3
subq %sp, %rlx, %rlx
stb %rlx, %al
outb %al
.endof
