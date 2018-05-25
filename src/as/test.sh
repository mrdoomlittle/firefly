./ffas -f ffef -i test.asm -o test.o
../bond/ffbond -s test.o -d a.out
../exec/ffexec a.out
#../exec/exec a.out
#./as -f ffef -i main.asm -o main.o && xxd main.o
#./as -f ffef -i ts0.asm -o ts0.o && xxd ts0.o
#./as -f ffef -i ts1.asm -o ts1.o && xxd ts1.o
