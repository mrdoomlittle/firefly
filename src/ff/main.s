.region text
ts0:
outb %ll
call $ts1
ret
ts1:
outb %ll
call $ts2
ret
ts2:
outb %ll
call $ts3
ret
ts3:
outb %ll
ret
_start:
asb %ll, 21
call $ts0
exit %ll
.endof
