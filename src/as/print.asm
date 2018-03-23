.globl print
.region text
print:
asb %al, 21
outb %al
exit %al
.axe
