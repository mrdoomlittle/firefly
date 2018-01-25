defines=-Wall -D__ffly_use_task_pool
nasm_flags=-Iasm/
ffly_cc=gcc
dst_dir=${PWD}
ffly_objs=$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=${PWD};. ./compile.sh;echo $$ffly_objs) \
$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=memory;cd ../;. ./memory/compile.sh;cd system;echo $$ffly_objs) \
$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=data;cd ../;. ./data/compile.sh;cd system;echo $$ffly_objs)
pipe: clean
	gcc -o n0 n0.c $(ffly_objs)
	gcc -o n1 n1.c $(ffly_objs)
sched: clean
	gcc sched.c $(ffly_objs)
test:
	gcc -c string.c
	gcc test.c string.o
print: clean
	nasm -f elf64 asm/sys_write.asm -o sys_write.o.0
	gcc -c sys_write.c -o sys_write.o.1
	gcc -c ../data/str_len.c
	gcc -c io.c 
	gcc -c file.c
	gcc -c errno.c
	gcc -c string.c
	gcc -c ../memory/mem_alloc.c
	gcc -c ../memory/mem_free.c
	gcc -c ../memory/mem_realloc.c
	gcc -c ../data/str_cpy.c
	gcc -D__ffly_no_task_pool -c ../data/mem_set.c
	gcc -D__ffly_no_task_pool -c ../data/mem_cpy.c
	gcc -c ../data/mem_dupe.c
	gcc printf.c string.o str_cpy.o errno.o mem_dupe.o mem_cpy.o mem_set.o mem_free.o mem_alloc.o mem_realloc.o file.o io.o str_len.o sys_write.o.0 sys_write.o.1 -o main
debug: clean
	gcc debug.c $(ffly_objs)
all: clean
	nasm -f elf64 $(nasm_flags) -o cond_lock.o asm/cond_lock.asm
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 asm/mutex.asm
	nasm -f elf64 $(nasm_flags) -o atomic_op.o.0 asm/atomic_op.asm
	nasm -f elf64 $(nasm_flags) -o signal.o asm/signal.asm
	nasm -f elf64 $(nasm_flags) -o sys_nanosleep.o.0 asm/sys_nanosleep.asm
	gcc -c $(defines) -std=c11 ../ffly_system.c
	gcc -c $(defines) -std=c11 config.c
	gcc -c $(defines) -std=gnu11 time.c
	gcc -c $(defines) -std=c11 flags.c
	gcc -c $(defines) -std=c11 arr.c
	gcc -c $(defines) -std=c11 ../data/mem_set.c
	gcc -c $(defines) -std=c11 ../data/mem_cpy.c
	gcc -c $(defines) -std=c11 task_pool.c
	gcc -c $(defines) -std=c11 task_worker.c
	gcc -c $(defines) -std=c11 task.c
	gcc -c $(defines) -std=c11 ../data/mem_dupe.c
	gcc -c $(defines) -std=c11 ../data/str_len.c
	gcc -c $(defines) -std=gnu11 thread.c
#	gcc -c $(defines) -std=c11 mem_tracker.c
#	g++ -std=c++11 main.cpp io.o arr.o mem_cpy.o mem_dupe.o mem_alloc.o mem_free.o mem_realloc.o thread.o errno.o -o main -lpthread
	gcc -c $(defines) -std=c11 errno.c
	gcc -c $(defines) -std=c11 ../memory/mem_alloc.c
	gcc -c $(defines) -std=c11 ../memory/mem_free.c
	gcc -c $(defines) -std=c11 ../memory/mem_realloc.c
#	gcc -c $(defines) -std=c11 mem_allocr.c
	gcc -c $(defines) -std=c11 vec.c
	gcc -c $(defines) -std=c11 io.c
	gcc -c $(defines) -std=c11 buff.c
	gcc -c $(defines) -std=c11 queue.c
	gcc -c $(defines) -std=c11 mem_blk.c
	gcc -c $(defines) -std=gnu11 futex.c
#	gcc -c $(defines) -std=c11 ../data/mem_ncpy.c
	gcc -c $(defines) -std=c11 ../data/mem_cmp.c
	gcc -c $(defines) -std=c11 util/hash.c
	gcc -c $(defines) -std=c11 bin_tree.c
	gcc -c $(defines) -std=c11 map.c
	gcc -c $(defines) -std=c11 mutex.c -o mutex.o.1
	gcc -c $(defines) -std=c11 atomic_op.c -o atomic_op.o.1
	gcc -c $(defines) -std=c11 sys_nanosleep.c -o sys_nanosleep.o.1
	gcc -c $(defines) -std=c11 ../data/mem_swp.c
	gcc -c $(defines) -std=c11 ../memory/temp.c
	gcc -c $(defines) -std=c11 ../data/bzero.c
##	gcc -c -std=c11 $(defines) -o mem_tracker.o mem_tracker.c
	gcc -std=c11 $(defines) -o main config.o ffly_system.o task.o map.o temp.o bzero.o sys_nanosleep.o.1 atomic_op.o.1 mutex.o.1 bin_tree.o hash.o str_len.o mem_cmp.o main.c futex.o mem_blk.o time.o sys_nanosleep.o.0 cond_lock.o atomic_op.o.0 mutex.o.0 flags.o mem_set.o mem_swp.o buff.o queue.o vec.o mem_cpy.o io.o errno.o mem_alloc.o mem_free.o mem_realloc.o arr.o mem_dupe.o thread.o task_pool.o task_worker.o signal.o -lm -lpthread
mutex:
	nasm -f elf64 -o mutex.o asm/mutex.asm
	gcc -std=c11 mutex.c mutex.o -lpthread
atomic:
	nasm -f elf64 -o atomic_op.o asm/atomic_op.asm
	gcc -std=c11 atomic_op.c atomic_op.o
nanosleep:
	nasm -f elf64 -o sys_nanosleep.o asm/sys_nanosleep.asm
	gcc -std=c11 sys_nanosleep.c sys_nanosleep.o
clean:
	rm -f *.o
#all:
#	g++ -c -std=c++11 ../memory/mem_alloc.cpp
#//	g++ -c -std=c++11 ../memory/mem_free.cpp
#//	g++ -c -std=c++11 ../memory/mem_realloc.cpp
#	gcc -c -std=c11 $(defines) errno.c
#	gcc -c -std=c11 $(defines) ../memory/mem_free.c
#	gcc -c -std=c11 $(defines) ../memory/mem_alloc.c
#	gcc -c -std=c11 $(defines) ../memory/mem_realloc.c
#	g++ -c -std=c++11 $(defines) -o aa.o ../memory/mem_alloc.cpp
#	g++ -c -std=c++11 $(defines) -o bb.o ../memory/mem_free.cpp
#	g++ -c -std=c++11 $(defines) -o cc.o ../memory/mem_realloc.cpp
#	gcc -c -std=c11 $(defines) -o mem_allocr.o mem_allocr.c
#	gcc -c -std=c11 $(defines) -o mem_tracker.o mem_tracker.c
#	gcc -c -std=c11 $(defines) -o smem_buff.o smem_buff.c
#	g++ -std=c++11 main.cpp $(defines) errno.o mem_alloc.o mem_free.o cc.o mem_tracker.o smem_buff.o aa.o bb.o mem_allocr.o mem_realloc.o
