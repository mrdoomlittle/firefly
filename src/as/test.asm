.region data
.segment
a:
.b 212
b:
.b 121
c:
.b 211
.endof
.region text
_start:
outb $c
asb %al, 0x2
exit %al
.endof
