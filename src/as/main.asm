.label test
.region text
_start:

jmp $test
test:
asb %al, 21
addb %al, %al, %al
exit %al
.axe
