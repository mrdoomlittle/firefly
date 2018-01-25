ffly_cc=gcc
dst_dir=${PWD}
ffly_objs=$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=../system;. ../system/compile.sh;echo $$ffly_objs) 
nasm_flags=-I../system/asm/
mem_cpy: clean
	gcc -c -Wall -std=c11 ../system/io.c
	gcc -c -Wall -std=c11 ../data/mem_cpy.c
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 ../system/asm/mutex.asm
	gcc -c -Wall -std=c11 -o mutex.o.1 ../system/mutex.c
	ld -r -o mutex.o mutex.o.0 mutex.o.1
	gcc -std=gnu11 -o mem_cpy mem_cpy.c mem_cpy.o mutex.o io.o
mem_dupe: clean
	gcc -c -Wall -std=c11 ../system/io.c
	gcc -c -Wall -std=c11 ../system/errno.c
	gcc -c -Wall -std=c11 ../data/mem_cpy.c
	gcc -c -Wall -std=c11 ../data/mem_dupe.c
	gcc -c -Wall -std=c11 ../memory/mem_alloc.c
	gcc -c -Wall -std=c11 ../memory/mem_free.c
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 ../system/asm/mutex.asm
	gcc -c -Wall -std=c11 -o mutex.o.1 ../system/mutex.c
	ld -r -o mutex.o mutex.o.0 mutex.o.1
	gcc -std=gnu11 -o mem_dupe mem_dupe.c errno.o mutex.o io.o mem_cpy.o mem_dupe.o mem_alloc.o mem_free.o
mem_cmp: clean
	gcc -c -Wall -std=c11 ../system/io.c
	gcc -c -Wall -std=c11 ../data/mem_cmp.c
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 ../system/asm/mutex.asm
	gcc -c -Wall -std=c11 -o mutex.o.1 ../system/mutex.c
	ld -r -o mutex.o mutex.o.0 mutex.o.1
	gcc -std=gnu11 -o mem_cmp mem_cmp.o mem_cmp.c io.o mutex.o
find: clean
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 ../system/asm/mutex.asm
	gcc -c -Wall -std=c11 -o mutex.o.1 ../system/mutex.c
	ld -r -o mutex.o mutex.o.0 mutex.o.1
	gcc -c -Wall -std=c11 ../data/mem_cmp.c
	gcc -c -Wall -std=c11 ../data/find.c
	gcc -c -Wall -std=c11 ../system/io.c
	gcc -std=gnu11 -o find find.c find.o mem_cmp.o io.o mutex.o

vec: clean
	gcc -D__ffly_no_task_pool -c -Wall -std=c11 ../data/mem_cpy.c
	gcc -c -Wall -std=c11 ../memory/mem_alloc.c
	gcc -c -Wall -std=c11 ../memory/mem_free.c
	gcc -c -Wall -std=c11 ../memory/mem_realloc.c
	gcc -c -Wall -std=c11 ../data/mem_cmp.c
	gcc -c -Wall -std=c11 ../data/str_len.c
	gcc -c -Wall -std=c11 ../data/mem_dupe.c
	gcc -c -Wall -std=c11 ../data/str_cpy.c
	gcc -c -Wall -std=c11 ../data/bcopy.c
	gcc -D__ffly_no_task_pool -c -Wall -std=c11 ../data/mem_set.c
	gcc -std=gnu11 -o vec.0 vec.0.c bcopy.o mem_cmp.o mem_set.o str_len.o mem_dupe.o str_cpy.o $(ffly_objs) mem_cpy.o mem_alloc.o mem_free.o mem_realloc.o
	gcc -std=gnu11 -o vec.1 vec.1.c bcopy.o mem_cmp.o mem_set.o str_len.o mem_dupe.o str_cpy.o $(ffly_objs) mem_cpy.o mem_alloc.o mem_free.o mem_realloc.o
set: clean
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 ../system/asm/mutex.asm
	gcc -c -Wall -std=c11 -o mutex.o.1 ../system/mutex.c
	ld -r -o mutex.o mutex.o.0 mutex.o.1
	gcc -c -Wall -std=c11 ../system/io.c
	gcc -c -Wall -std=c11 ../system/errno.c
	gcc -c -Wall -std=c11 ../data/mem_cpy.c
	gcc -c -Wall -std=c11 ../memory/mem_alloc.c
	gcc -c -Wall -std=c11 ../memory/mem_free.c
	gcc -c -Wall -std=c11 ../memory/mem_realloc.c
	gcc -c -Wall -std=c11 ../system/flags.c
	gcc -c -Wall -std=c11 ../system/vec.c
	gcc -c -Wall -std=c11 ../system/util/hash.c
	gcc -c -Wall -std=c11 ../system/bin_tree.c
	gcc -c -Wall -std=c11 ../data/mem_swp.c
	g++ -std=c++11 -o set set.cpp mem_swp.o hash.o bin_tree.o io.o flags.o vec.o errno.o mem_cpy.o mutex.o mem_alloc.o mem_free.o mem_realloc.o
all: mem_cpy mem_dupe mem_cmp find vec set
clean:
	rm -f *.o mem_cpy mem_dupe mem_cmp find vec.0 vec.1 set
