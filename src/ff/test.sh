#../ffc -g resin -i test.ff -o test.s
../as/ffas -f ffef -i test.s -o test.o
#../as/ffas -f ffef -i lib/start.s -o start.o
#../bond/ffbond -s "test.o" -d a.out
