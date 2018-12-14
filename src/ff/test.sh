../ffc -g amd64 -i test.ff -o test.s
../as/ffas -f elf -i test.s -o test.o -p amd64
ld test.o -o a.out
#../as/ffas -f ffef -i lib/start.s -o start.o
#../bond/ffbond -s "test.o" -d a.out
