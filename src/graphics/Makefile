USING=__USING_X11
LFLAGS=-L/usr/lib/x86_64-linux-gnu
all: clean
	gcc -Wall -c -std=c11 ../system/buff.c
	gcc -Wall -c -std=c11 ../system/queue.c
	gcc -Wall -c -std=c11 ../memory/mem_realloc.c
	nasm -f elf64 -o cond_lock.o ../system/asm/cond_lock.asm
	nasm -f elf64 -o atomic_op.o ../system/asm/atomic_op.asm
	nasm -f elf64 -o mutex.o ../system/asm/mutex.asm
	nasm -f elf64 -o sys_nanosleep.o ../system/asm/sys_nanosleep.asm
	nasm -f elf64 -o signal.o ../system/asm/signal.asm
	gcc -Wall -c -std=c11 ../system/mem_blk.c
	gcc -Wall -c -std=c11 ../memory/mem_free.c
	gcc -Wall -c -std=gnu11 ../system/thread.c
	gcc -Wall -c -std=c11 ../data/mem_set.c
	gcc -Wall -c -std=c11 ../memory/alloc_pixelmap.c
	gcc -Wall -c -std=c11 ../data/str_len.c
	gcc -Wall -c -std=c11 ../memory/mem_alloc.c
	gcc -Wall -c -std=c11 ../system/errno.c
	gcc -Wall -c -std=c11 ../data/mem_dupe.c
	gcc -Wall -c -std=c11 ../data/mem_cpy.c
	gcc -Wall -c -std=c11 ../system/io.c
	gcc -Wall -c -std=c11 x11_wd.c
	gcc -Wall -c -std=c11 xcb_wd.c
	gcc -Wall -c -std=c11 ../system/flags.c
	g++ -Wall -std=c++11 -D$(USING) $(LFLAGS) -o main cond_lock.o sys_nanosleep.o mem_blk.o signal.o main.cpp queue.o buff.o atomic_op.o mem_free.o mem_realloc.o mutex.o thread.o flags.o mem_set.o window.cpp mem_alloc.o errno.o io.o str_len.o x11_wd.o xcb_wd.o mem_dupe.o mem_cpy.o /usr/lib/x86_64-linux-gnu/libGLU.so.1 -lX11 -lGL -lglut -lX11-xcb -lxcb -lxcb-icccm -lpthread
clean:
	rm -f *.o
