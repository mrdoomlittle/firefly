../ffc -i main.ff -o main.s
../as/as -f ffef -i main.s -o main.o
../as/as -f ffef -i start.s -o start.o
../bond/bond -s "start.o main.o" -d a.out
