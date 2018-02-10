ffly_objs="output.o x86.o string.o as.o hash.o str_cmp.o str_len.o mem_cmp.o bzero.o hash.0 alloca.o parser.o exp.o"
rm -f *.o
gcc -c output/x86.c
gcc -c ../system/string.c
gcc -c ../data/str_cmp.c 
gcc -c ../data/str_len.c
gcc -c ../data/mem_cmp.c
gcc -c ../data/bzero.c
gcc -c ../system/util/hash.c
gcc -c alloca.c
gcc -c parser.c
gcc -c exp.c
gcc -c as.c
gcc -c hash.c -o hash.0
gcc -c -o output.o output.c
gcc -o as main.c $ffly_objs
