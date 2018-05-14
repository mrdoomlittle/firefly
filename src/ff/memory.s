.region text
.globl ffly_mem_alloc
ffly_mem_alloc:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 8
subq %sp, %rlx, %sp
asq %rel, 0
asq %rlx, 8
subq %bp, %rlx, %rlx
std %rlx, %ael
movd %ael, ael
rin 3, %ael
.l l0
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ffly_mem_free
ffly_mem_free:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 8
subq %sp, %rlx, %sp
asq %rel, 0
asq %rlx, 8
subq %bp, %rlx, %rlx
stq %rlx, %rel
movq %rel, rel
rin 4, %rel
.l l1
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ffly_mem_read
ffly_mem_read:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 16
subq %sp, %rlx, %sp
.l l2
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.globl ffly_mem_write
ffly_mem_write:
;save %bp
asq %rlx, 8
subq %sp, %rlx, %sp
ldq %sp, %bp
movq %sp, %bp
asq %rlx, 16
subq %sp, %rlx, %sp
.l l3
movq %bp, %sp
;reset %bp
stq %sp, %bp
asq %rlx, 8
addq %sp, %rlx, %sp
ret
.endof
