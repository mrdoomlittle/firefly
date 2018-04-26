.region text
.globl test
test:
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
movq %bp, %sp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
