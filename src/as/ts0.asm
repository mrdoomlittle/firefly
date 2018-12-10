.extern ts0
.extern ts1
.region text
_start:
	call $ts0
	call $ts1
	asb %al, 21
	exit %al
.endof
