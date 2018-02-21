nasm_flags=-I../system/asm/ -I../linux/asm/
objs=ffly_system.o allocr.o flags.o vec.o checksum.o sys_fcntl.o.0 sys_fcntl.o.1 \
sys_open.o.0 sys_open.o.1 sys_close.o.0 sys_close.o.1 printf.o sys_brk.o.0 sys_brk.o.1 string.o str_cpy.o \
errno.o mem_dupe.o mem_cpy.o mem_set.o mem_free.o mem_alloc.o mem_realloc.o file.o io.o str_len.o \
sys_write.o.0 sys_write.o.1 mutex.o.0 mutex.o.1 atomic_op.o.0 atomic_op.o.1 cond_lock.o \
sys_nanosleep.o.0 sys_nanosleep.o.1 bzero.o buff.o err.o \
sys_mmap.o.0 sys_mmap.o.1 sys_munmap.o.0 sys_munmap.o.1 \
sys_getpid.o.0 sys_getpid.o.1 sys_kill.o.0 sys_kill.o.1 \
sys_access.o.0 sys_access.o.1 sys_fsync.o.0 sys_fsync.o.1 \
sys_read.o.0 sys_read.o.1 sys_stat.o.0 sys_stat.o.1 \
sys_lseek.o.0 sys_lseek.o.1 sys_creat.o.0 sys_creat.o.1 \
sys_exit.o.0 sys_exit.o.1 rand.o
ccflags=-D__ffly_use_allocr -fno-builtin
all: clean
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_exit.asm -o sys_exit.o.0
	gcc $(ccflags) -c ../system/sys_exit.c -o sys_exit.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_stat.asm -o sys_stat.o.0
	gcc $(ccflags) -c ../system/sys_stat.c -o sys_stat.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_lseek.asm -o sys_lseek.o.0
	gcc $(ccflags) -c ../system/sys_lseek.c -o sys_lseek.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_creat.asm -o sys_creat.o.0
	gcc $(ccflags) -c ../system/sys_creat.c -o sys_creat.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_read.asm -o sys_read.o.0
	gcc $(ccflags) -c ../system/sys_read.c -o sys_read.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_access.asm -o sys_access.o.0
	gcc $(ccflags) -c ../system/sys_access.c -o sys_access.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_fsync.asm -o sys_fsync.o.0
	gcc $(ccflags) -c ../system/sys_fsync.c -o sys_fsync.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_getpid.asm -o sys_getpid.o.0
	gcc $(ccflags) -c ../system/sys_getpid.c -o sys_getpid.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_kill.asm -o sys_kill.o.0
	gcc $(ccflags) -c ../system/sys_kill.c -o sys_kill.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_mmap.asm -o sys_mmap.o.0
	gcc $(ccflags) -c ../system/sys_mmap.c -o sys_mmap.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_munmap.asm -o sys_munmap.o.0
	gcc $(ccflags) -c ../system/sys_munmap.c -o sys_munmap.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/mutex.asm -o mutex.o.0
	gcc $(ccflags) -c ../system/mutex.c -o mutex.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/atomic_op.asm -o atomic_op.o.0
	gcc $(ccflags) -c ../system/atomic_op.c -o atomic_op.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_fcntl.asm -o sys_fcntl.o.0
	gcc $(ccflags) -c ../system/sys_fcntl.c -o sys_fcntl.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_open.asm -o sys_open.o.0
	gcc $(ccflags) -c ../system/sys_open.c -o sys_open.o.1

	nasm $(nasm_flags) -f elf64 ../system/asm/sys_close.asm -o sys_close.o.0
	gcc $(ccflags) -c ../system/sys_close.c -o sys_close.o.1

	nasm $(nasm_flags) -f elf64 -o cond_lock.o ../system/asm/cond_lock.asm
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_write.asm -o sys_write.o.0
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_brk.asm -o sys_brk.o.0
	nasm $(nasm_flags) -f elf64 ../system/asm/sys_nanosleep.asm -o sys_nanosleep.o.0
	gcc $(ccflags) -c ../system/sys_brk.c -o sys_brk.o.1
	gcc $(ccflags) -c ../system/sys_write.c -o sys_write.o.1
	gcc $(ccflags) -c ../system/sys_nanosleep.c -o sys_nanosleep.o.1
	gcc $(ccflags) -c ../data/str_len.c
	gcc $(ccflags) -c ../system/io.c 
	gcc $(ccflags) -c ../system/file.c
	gcc $(ccflags) -c ../system/errno.c
	gcc $(ccflags) -c ../system/string.c
	gcc $(ccflags) -D__ffly_use_allocr -c ../memory/mem_alloc.c
	gcc $(ccflags) -D__ffly_use_allocr -c ../memory/mem_free.c
	gcc $(ccflags) -D__ffly_use_allocr -c ../memory/mem_realloc.c
	gcc $(ccflags) -c ../data/str_cpy.c
	gcc $(ccflags) -D__ffly_no_task_pool -c ../data/mem_set.c
	gcc $(ccflags) -D__ffly_no_task_pool -c ../data/mem_cpy.c
	gcc $(ccflags) -c ../data/mem_dupe.c
	gcc $(ccflags) -D__ffly_debug -c ../system/printf.c
	gcc $(ccflags) -c -std=gnu99 allocr.c
	gcc $(ccflags) -c ../system/util/checksum.c
	gcc $(ccflags) -c ../system/vec.c
	gcc $(ccflags) -c ../system/flags.c
	gcc $(ccflags) -c ../data/bzero.c
	gcc $(ccflags) -c ../system/err.c
	gcc $(ccflags) -c ../rand.c
	gcc $(ccflags) -D__ffly_no_sysconf -D__ffly_no_task_pool -D__ffly_no_thread -c ../ffly_system.c
#	gcc -D__ffly_no_sysconf -D__ffly_use_allocr -c ../system/thread.c
	gcc $(ccflags) -c ../system/buff.c
	#gcc $(objs) -o libmain.so
	gcc $(ccflags) -o main main.c $(objs) -nostdlib
ts0: cl
	gcc -o ts0 ts0.c $(objs)
ts1: cl
	gcc -o ts1 ts1.c $(objs)
clean:
	rm -f *.o *.o*
cl:
	rm -f ts0 ts1
