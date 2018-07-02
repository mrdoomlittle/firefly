	.region text
	.extern test
_start:
	call $test
	exit %al
.endof
