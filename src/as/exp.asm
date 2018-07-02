
;rin 0, 0
r0r0
r1r1
asq %r0, 0xffffffffff
asq %r1, 0xffffffffff
pushq %r0
pushq %r1
asq %r0, 0x0
asq %r1, 0x0
popq %r0
popq %r1
outq %r0
outq %r1
asb %r0, 0xf

exit %r0
