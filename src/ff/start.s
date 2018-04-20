.region text
.extern main
.globl _start
_start:
call $main
exit %ll
.endof
