cd ../bcd && sh compile.sh && cd ../as
sh compile.sh && ./as -i test.asm -o test.o
../bcd/bcd test.o

