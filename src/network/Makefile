all: clean
	gcc -c ../system/io.c
	nasm -f elf64 -I../system/asm/ -o mutex.o.0 ../system/asm/mutex.asm
	gcc -c -std=c11 -o mutex.o.1 ../system/mutex.c
	nasm -f elf64 -o sys_nanosleep.o.0 ../system/asm/sys_nanosleep.asm
	gcc -c -o sys_nanosleep.o.1 ../system/sys_nanosleep.c
	gcc -c sock.c
	gcc -c http.c
	gcc -c ../data/mem_cpy.c
	gcc -c ../data/str_cpy.c
	gcc -c ../data/str_len.c
	gcc -std=gnu11 -c resolve.c
	gcc -c ../data/bcopy.c
	gcc -c ../data/bzero.c
	gcc -c ../memory/mem_alloc.c
	gcc -c ../memory/mem_free.c
	gcc -c ../memory/mem_realloc.c
	gcc -c ../data/mem_cmp.c
	gcc -c ../data/str_cmp.c
	gcc -o main main.c bcopy.o sys_nanosleep.o.0 sys_nanosleep.o.1 mem_realloc.o mem_cmp.o str_cmp.o mem_alloc.o mem_free.o bzero.o mem_cpy.o str_cpy.o str_len.o resolve.o http.o sock.o io.o mutex.o.0 mutex.o.1
clean:
	rm -f *.o *.o*
