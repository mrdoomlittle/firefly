.region text
_start:
	jmp $ts1
	asb %al, 2
	exit %al
ts1:
	asb %al, 4
	exit %al
.endof
