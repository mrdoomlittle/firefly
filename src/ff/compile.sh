../ffc -i main.ff -o main.s
../ffc -i lib/error.ff -o error.s
../ffc -i lib/memory.ff -o memory.s
../as/ffas -f ffef -i main.s -o main.o
../as/ffas -f ffef -i error.s -o error.o
../as/ffas -f ffef -i memory.s -o memory.o
../as/ffas -f ffef -i lib/start.s -o start.o
../bond/ffbond -s "start.o main.o error.o memory.o" -d a.out
