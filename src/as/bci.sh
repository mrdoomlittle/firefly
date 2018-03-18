cd ../bcd && sh compile.sh && cd ../as
sh compile.sh && ./as -f ffef -i test.asm -o test.o
../bcd/bcd test.o

