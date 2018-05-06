	.region text
	.extern main
; entry point
_start:
	rin 0, 0; set stack pointer register - %sp
	call $main; call user function
; exit with code 21
	asb %al, 21;
	exit %al
.endof
