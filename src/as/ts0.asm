.extern main
.region text
_start:
	call $main
	asb %al, 21
	exit %al
.endof
