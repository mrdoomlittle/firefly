.extern test
.region text
proxy:
asb %al, 86
outb %al
jmp $test
_start:
asb %al, 56
outb %al
jmp $proxy
.axe
