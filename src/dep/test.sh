gcc -c str_len.c
gcc -c mem_cpy.c
gcc -c mem_mov.c
gcc -c mem_dup.c
gcc -c str_cpy.c
gcc -c bzero.c
gcc -c bcopy.c
gcc -c str_dup.c
gcc -c str_cmp.c
gcc -c mem_cmp.c
gcc main.c *.s *.o -o main
