.region text
_start:
	jmp $ts4
	jmp $ts3
	jmp $ts2
	jmp $ts0
	ts0:
	ts1:
	ts2:
	ts3:
	ts4:
	asb %al, 0
	exit %al
.endof
