.globl test
.region text
test:
asb %al, 21
outb %al
exit %al
.axe
