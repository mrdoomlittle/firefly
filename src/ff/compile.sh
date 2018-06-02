../ffc -g resin -i main.ff -o main.s
#../ffc -g resin -i lib/error.ff -o error.s
#../ffc -g resin -i lib/memory.ff -o memory.s
../ffc -g resin -i lib/init.ff -o init.s

../as/ffas -f ffef -i main.s -o main.o
#../as/ffas -f ffef -i error.s -o error.o
#../as/ffas -f ffef -i memory.s -o memory.o
../as/ffas -f ffef -i init.s -o init.o
../as/ffas -f ffef -i lib/start.s -o start.o
../bond/ffbond -s "start.o main.o init.o" -d a.out
