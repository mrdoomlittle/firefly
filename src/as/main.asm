.label test; define label
.label again
.region text
_start:
	call $test; call test
	exit %al; exit
test:
	asb %ll, 0; set to zero
again:
	incb %ll; incrment
	asb %al, 20; assign 20 to al register
	cmpb %ll, %al, %ae; compare ll and al registers
	outb %ll; output ll value
	jne $again, %ae; jump if not equal
	movb %ll, %al; move ll to al
	ret; return
.endof
