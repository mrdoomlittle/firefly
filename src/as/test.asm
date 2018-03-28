.extern test
.globl testa
.globl testb
.region text

testa:
testb:

_start:
jmp $test
exit 0x0
.axe
