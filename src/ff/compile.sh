../ffc -g resin -i main.ff -o main.s
#../ffc -g resin -i lib/error.ff -o error.s
#../ffc -g resin -i lib/memory.ff -o memory.s
../ffc -g resin -i lib/init.ff -o init.s

../as/ffas -f remf -i main.s -o main.o
#../as/ffas -f remf -i error.s -o error.o
#../as/ffas -f remf -i memory.s -o memory.o
../as/ffas -f remf -i init.s -o init.o
../as/ffas -f remf -i lib/start.s -o start.o
../bond/ffbond -s "main.o init.o start.o" -d a.out
