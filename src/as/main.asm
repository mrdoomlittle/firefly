.label test; define label
.label again
.region text
_start:

jmp $test
test:
outb %ll
again:
incb %ll

asb %al, 20
cmpb %ll, %al, %ae
outb %ll
jne $again, %ae
exit %al
.axe
