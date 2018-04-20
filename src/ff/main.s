.region text
.globl ts0
ts0:
outb %ll
call $ts1
ret
.globl ts1
ts1:
outb %ll
call $ts2
ret
.globl ts2
ts2:
outb %ll
call $ts3
ret
.globl ts3
ts3:
outb %ll
ret
.globl main
main:
asb %ll, 21
call $ts0
ret
.endof
