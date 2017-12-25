DEFINES=
#-D__WITH_MEM_TRACKER
all: clean
	nasm -f elf64 -o cond_lock.o asm/cond_lock.asm
	nasm -f elf64 -o mutex.o asm/mutex.asm
	nasm -f elf64 -o atomic_op.o asm/atomic_op.asm
	nasm -f elf64 -o signal.o asm/signal.asm
	nasm -f elf64 -o sys_nanosleep.o asm/sys_nanosleep.asm
	gcc -c $(DEFINES) -std=gnu11 time.c
	gcc -c $(DEFINES) -std=c11 flags.c
	gcc -c $(DEFINES) -std=c11 arr.c
	gcc -c $(DEFINES) -std=c11 ../data/mem_set.c
	gcc -c $(DEFINES) -std=c11 ../data/mem_cpy.c
	gcc -c $(DEFINES) -std=c11 task_pool.c
	gcc -c $(DEFINES) -std=c11 task_worker.c
	gcc -c $(DEFINES) -std=c11 ../data/mem_dupe.c
	gcc -c $(DEFINES) -std=c11 ../data/str_len.c
	gcc -c $(DEFINES) -std=gnu11 thread.c
#	gcc -c $(DEFINES) -std=c11 mem_tracker.c
#	g++ -std=c++11 main.cpp io.o arr.o mem_cpy.o mem_dupe.o mem_alloc.o mem_free.o mem_realloc.o thread.o errno.o -o main -lpthread
	gcc -c $(DEFINES) -std=c11 errno.c
	gcc -c $(DEFINES) -std=c11 ../memory/mem_alloc.c
	gcc -c $(DEFINES) -std=c11 ../memory/mem_free.c
	gcc -c $(DEFINES) -std=c11 ../memory/mem_realloc.c
#	gcc -c $(DEFINES) -std=c11 mem_allocr.c
	gcc -c $(DEFINES) -std=c11 vec.c
	gcc -c $(DEFINES) -std=c11 io.c
	gcc -c $(DEFINES) -std=c11 buff.c
	gcc -c $(DEFINES) -std=c11 queue.c
	gcc -c $(DEFINES) -std=c11 mem_blk.c
	gcc -c $(DEFINES) -std=gnu11 futex.c
#	gcc -c $(DEFINES) -std=c11 ../data/mem_ncpy.c
	gcc -c $(DEFINES) -std=c11 ../data/mem_cmp.c
	gcc -c $(DEFINES) -std=c11 util/hash.c
	gcc -c $(DEFINES) -std=c11 bin_tree.c
	gcc -c $(DEFINES) -std=c11 map.c
	gcc -c $(DEFINES) -std=c11 ../data/mem_swp.c
##	gcc -c -std=c11 $(DEFINES) -o mem_tracker.o mem_tracker.c
	g++ -std=c++11 $(DEFINES) -o main map.o bin_tree.o hash.o str_len.o mem_cmp.o main.cpp futex.o mem_blk.o time.o sys_nanosleep.o cond_lock.o atomic_op.o mutex.o flags.o mem_set.o mem_swp.o buff.o queue.o vec.o mem_cpy.o io.o errno.o mem_alloc.o mem_free.o mem_realloc.o arr.o mem_dupe.o thread.o task_pool.o task_worker.o signal.o -lpthread
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
#	gcc -c -std=c11 $(DEFINES) errno.c
#	gcc -c -std=c11 $(DEFINES) ../memory/mem_free.c
#	gcc -c -std=c11 $(DEFINES) ../memory/mem_alloc.c
#	gcc -c -std=c11 $(DEFINES) ../memory/mem_realloc.c
#	g++ -c -std=c++11 $(DEFINES) -o aa.o ../memory/mem_alloc.cpp
#	g++ -c -std=c++11 $(DEFINES) -o bb.o ../memory/mem_free.cpp
#	g++ -c -std=c++11 $(DEFINES) -o cc.o ../memory/mem_realloc.cpp
#	gcc -c -std=c11 $(DEFINES) -o mem_allocr.o mem_allocr.c
#	gcc -c -std=c11 $(DEFINES) -o mem_tracker.o mem_tracker.c
#	gcc -c -std=c11 $(DEFINES) -o smem_buff.o smem_buff.c
#	g++ -std=c++11 main.cpp $(DEFINES) errno.o mem_alloc.o mem_free.o cc.o mem_tracker.o smem_buff.o aa.o bb.o mem_allocr.o mem_realloc.o
