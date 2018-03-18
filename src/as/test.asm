
.segment bss
test:
.b 21
_start:
asq %rlx, $test
asb %al, 21
movb %al, %rlx
outq %al
rin %al
exit 0x0
