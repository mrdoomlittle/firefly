gcc -c str_len.c
gcc -c mem_cpy.c
gcc -c mem_mov.c
gcc main.c *.s mem_mov.o mem_cpy.o str_len.o -o main
