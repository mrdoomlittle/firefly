	.region text
	.globl test
test:
	asb %al, 212
	outb %al
	ret
.endof
