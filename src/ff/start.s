.region text
.extern main
.globl _start
_start:
asb %al, 0
rin %al
call $main
exit %ll
.endof
