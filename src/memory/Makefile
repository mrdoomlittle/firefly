nasm_flags=-I../system/asm/
objs=ffly_system.o allocr.o thread.o flags.o vec.o checksum.o sys_fcntl.o.0 sys_fcntl.o.1 \
sys_open.o.0 sys_open.o.1 sys_close.o.0 sys_close.o.1 printf.o sys_brk.o.0 sys_brk.o.1 string.o str_cpy.o \
errno.o mem_dupe.o mem_cpy.o mem_set.o mem_free.o mem_alloc.o mem_realloc.o file.o io.o str_len.o \
sys_write.o.0 sys_write.o.1 mutex.o.0 mutex.o.1 atomic_op.o.0 atomic_op.o.1 cond_lock.o \
sys_nanosleep.o.0 sys_nanosleep.o.1 firefly.o
all: clean
	nasm $(nasm_flags) -f elf64 ../system/asm/mutex.asm -o mutex.o.0
	gcc -c ../system/mutex.c -o mutex.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/atomic_op.asm -o atomic_op.o.0
	gcc -c ../system/atomic_op.c -o atomic_op.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_fcntl.asm -o sys_fcntl.o.0
	gcc -c ../system/sys_fcntl.c -o sys_fcntl.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_open.asm -o sys_open.o.0
	gcc -c ../system/sys_open.c -o sys_open.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_close.asm -o sys_close.o.0
	gcc -c ../system/sys_close.c -o sys_close.o.1

	nasm $(nasm_flags) -f elf64 -o cond_lock.o ../system/asm/cond_lock.asm
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_write.asm -o sys_write.o.0
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_brk.asm -o sys_brk.o.0
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_nanosleep.asm -o sys_nanosleep.o.0
	gcc -c ../system/sys_brk.c -o sys_brk.o.1
	gcc -c ../system/sys_write.c -o sys_write.o.1
	gcc -c ../system/sys_nanosleep.c -o sys_nanosleep.o.1
	gcc -c ../data/str_len.c
	gcc -c ../system/io.c 
	gcc -c ../system/file.c
	gcc -c ../system/errno.c
	gcc -c ../system/string.c
	gcc -D__ffly_use_allocr -c ../memory/mem_alloc.c
	gcc -D__ffly_use_allocr -c ../memory/mem_free.c
	gcc -D__ffly_use_allocr -c ../memory/mem_realloc.c
	gcc -c ../data/str_cpy.c
	gcc -D__ffly_no_task_pool -c ../data/mem_set.c
	gcc -D__ffly_no_task_pool -c ../data/mem_cpy.c
	gcc -c ../data/mem_dupe.c
	gcc -D__ffly_debug -c ../system/printf.c
	gcc -c -std=gnu99 allocr.c
	gcc -c ../system/util/checksum.c
	gcc -c ../system/vec.c
	gcc -c ../system/flags.c
	gcc -c ../firefly.c
	gcc -D__ffly_no_sysconf -D__ffly_no_task_pool -c ../ffly_system.c
	gcc -D__ffly_no_sysconf -D__ffly_use_allocr -c ../system/thread.c
	gcc -o  main main.c $(objs)
ts0: cl
	gcc -o ts0 ts0.c $(objs)
ts1: cl
	gcc -o ts1 ts1.c $(objs)
clean:
	rm -f *.o *.o*
cl:
	rm -f ts0 ts1
