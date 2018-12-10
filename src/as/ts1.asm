.globl ts0
.globl ts1
.region text
ts0:
	asb %al, 212
	outb %al
	ret
ts1:
	asb %al, 101
	outb %al
	ret
.endof
