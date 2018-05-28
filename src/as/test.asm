.region text
ts0:
ret
ts1:
ret
ts2:
ret
_start:
rin 0, 0
call $ts0
call $ts1
call $ts2
asb %al, 21
exit %al
.endof
