again:
asb %al, 21
outb %al
jmp again
asb %al, 0x0
exit 0x0
