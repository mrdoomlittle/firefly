./ffas -f remf -i test.asm -o test.o
#./ffas -f ffef -i func.asm -o func.o
#../readremf/readremf test.o
#../readffef/readffef func.o
#../bond/ffbond -s "test.o" -d a.out
#../exec/ffexec a.out
#./ffas -f ffef -i func.asm -o func.o
#echo "\n\n"
#../readffef/readffef test.o
#../readffef/readffef func.o
#../bond/ffbond -s "test.o func.o" -d a.out
#../exec/ffexec a.out
#../exec/exec a.out
#./as -f ffef -i main.asm -o main.o && xxd main.o
#./as -f ffef -i ts0.asm -o ts0.o && xxd ts0.o
#./as -f ffef -i ts1.asm -o ts1.o && xxd ts1.o
