.globl main
.region text
main:
	asb %al, 212
	outb %al
	ret
.endof
