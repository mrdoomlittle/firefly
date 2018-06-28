.extern test
.region text
_start:
asb %al, 21
call $test
exit %al
.endof
