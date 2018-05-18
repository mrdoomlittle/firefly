.region text
.extern ffly_mem_alloc
.extern ffly_mem_free
.extern ffly_mem_read
.extern ffly_mem_write
.globl main
main:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 10
subq %sp, %rlx, %sp
asq %rel, 0
asb %ae, 101
asq %rlx, 1
subq %bp, %rlx, %rlx
ldb %rlx, %ae
asq %rel, 0
asq %rel, 0
asb %ae, 200
asq %rlx, 26
subq %bp, %rlx, %rlx
ldd %rlx, %ael
call $ffly_mem_alloc
asq %rlx, 10
subq %bp, %rlx, %rlx
ldq %rlx, %rel
asq %rel, 0
asq %rel, 0
asq %rlx, 10
subq %bp, %rlx, %rlx
stq %rlx, %rel
asq %rlx, 26
subq %bp, %rlx, %rlx
ldq %rlx, %rel
asq %rel, 0
asq %rel, 1
subq %bp, %rel, %rel
asq %rlx, 34
subq %bp, %rlx, %rlx
ldq %rlx, %rel
asq %rel, 0
asq %rel, 0
asb %ae, 1
asq %rlx, 42
subq %bp, %rlx, %rlx
ldd %rlx, %ael
call $ffly_mem_write
asq %rel, 0
asq %rel, 0
asq %rlx, 10
subq %bp, %rlx, %rlx
stq %rlx, %rel
asq %rlx, 26
subq %bp, %rlx, %rlx
ldq %rlx, %rel
asq %rel, 0
asq %rel, 2
subq %bp, %rel, %rel
asq %rlx, 34
subq %bp, %rlx, %rlx
ldq %rlx, %rel
asq %rel, 0
asq %rel, 0
asb %ae, 1
asq %rlx, 42
subq %bp, %rlx, %rlx
ldd %rlx, %ael
call $ffly_mem_read
asq %rel, 0
asq %rlx, 2
subq %bp, %rlx, %rlx
stb %rlx, %ae
outb %ae
asq %rel, 0
asq %rel, 0
asq %rlx, 10
subq %bp, %rlx, %rlx
stq %rlx, %rel
asq %rlx, 26
subq %bp, %rlx, %rlx
ldq %rlx, %rel
call $ffly_mem_free
.l l0
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
