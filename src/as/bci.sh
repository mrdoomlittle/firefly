cd ../bcd && sh compile.sh && cd ../as
sh compile.sh && ./as -f bc -i test.asm -o test.o
../bcd/bcd test.o

