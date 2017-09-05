USING=__USING_X11
all:
	gcc -c -std=c11 ../memory/mem_free.c
	gcc -c -std=c11 ../system/thread.c
	gcc -c -std=c11 ../data/mem_set.c
	gcc -c -std=c11 ../memory/alloc_pixmap.c
	gcc -c -std=c11 ../data/str_len.c
	gcc -c -std=c11 ../memory/mem_alloc.c
	gcc -c -std=c11 ../system/errno.c
	gcc -c -std=c11 ../data/mem_dupe.c
	gcc -c -std=c11 ../data/mem_cpy.c
	gcc -c -std=c11 ../system/io.c
	gcc -c -std=c11 x11_wd.c
	gcc -c -std=c11 xcb_wd.c
	gcc -c -std=c11 ../system/flags.c
	g++ -std=c++11 -D$(USING) -o main mem_free.o thread.o flags.o mem_set.o window.cpp mem_alloc.o errno.o io.o str_len.o x11_wd.o xcb_wd.o mem_dupe.o mem_cpy.o -lX11 -lGL -lGLU -lglut -lX11-xcb -lxcb -lxcb-icccm -lpthread
