.extern test
.globl testa
.globl testb
testa:
testb
.region text
_start:
jmp $test
exit 0x0
.axe
