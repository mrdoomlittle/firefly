rm -f *.o
ccflags=-D__ffly_source
#gcc $ccflags -c str_len.c
#gcc $ccflags -c mem_cpy.c
#gcc $ccflags -c mem_mov.c
#gcc $ccflags -c mem_dup.c
#gcc $ccflags -c str_cpy.c
gcc $ccflags -c bzero.c
gcc $ccflags -c bcopy.c
#gcc $ccflags -c str_dup.c
#gcc $ccflags -c str_cmp.c
#gcc $ccflags -c mem_cmp.c
gcc $ccflags test.c bzero.s *.o -o main
