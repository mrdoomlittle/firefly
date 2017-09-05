mem_cpy: clean
	gcc -c -std=c11 ../data/mem_cpy.c
	gcc -std=gnu11 -o mem_cpy mem_cpy.c mem_cpy.o
mem_dupe: clean
	gcc -c -std=c11 ../system/errno.c
	gcc -c -std=c11 ../system/io.c
	gcc -c -std=c11 ../data/mem_cpy.c
	gcc -c -std=c11 ../data/mem_dupe.c
	gcc -c -std=c11 ../memory/mem_alloc.c
	gcc -c -std=c11 ../memory/mem_free.c
	gcc -std=gnu11 -o mem_dupe mem_dupe.c errno.o io.o mem_cpy.o mem_dupe.o mem_alloc.o mem_free.o
mem_cmp: clean
	gcc -c -std=c11 ../data/mem_cmp.c
	gcc -std=gnu11 -o mem_cmp mem_cmp.o mem_cmp.c
find: clean
	gcc -c -std=c11 ../data/mem_cmp.c
	gcc -c -std=c11 ../data/find.c
	gcc -std=gnu11 -o find find.c find.o mem_cmp.o
clean:
	rm -f *.o mem_cpy mem_dupe mem_cmp find
