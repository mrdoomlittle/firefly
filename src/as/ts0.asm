.extern ts0
.extern ts1
.region text
_start:
	call $ts0
	call $ts1
	jmp #go
	asb %al, 21


	exit %al
	exit %al
	exit %al
	exit %al
	exit %al
	exit %al
	exit %al
	exit %al
	exit %al
	exit %al
.l go
	asb %al, 2
	exit %al
.endof
