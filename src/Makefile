DEFINES=-I../eint_t/inc -D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
all:
	rm -f *.o
	nasm -f elf64 -o mutex.o system/asm/mutex.asm
	gcc -c -std=c11 -DARC64 $(DEFINES) maths/is_inside.c
	gcc -c -std=c11 -DARC64 $(DEFINES) system/vec.c
	gcc -c -std=c11 -DARC64 $(DEFINES) system/mem_tracker.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/alloc_pixmap.c
	gcc -c -std=c11 -DARC64 $(DEFINES) data/mem_cpy.c
	gcc -c -std=c11 -DARC64 $(DEFINES) data/swp.c
	gcc -c -std=c11 -DARC64 $(DEFINES) system/io.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/mem_alloc.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/mem_free.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/mem_realloc.c
	gcc -c -std=c11 -DARC64 $(DEFINES) -o chunk_keeper.o chunk_keeper.c
	g++ -c -std=c++11 -DARC64 $(DEFINES) -o chunk_manager.o chunk_manager.cpp
	gcc -c -std=c11 -DARC64 $(DEFINES) data/mem_cmp.c
	gcc -c -std=c11 -DARC64 $(DEFINES) data/find.c
	g++ -std=c++11 -DARC64 $(DEFINES) -o main main.cpp mutex.o is_inside.o vec.o find.o mem_tracker.o chunk_manager.o mem_cmp.o swp.o alloc_pixmap.o mem_cpy.o io.o chunk_keeper.o mem_alloc.o mem_free.o mem_realloc.o
script: clean
	gcc -c -std=c11 system/io.c
	gcc -c -std=c11 memory/mem_alloc.c
	gcc -c -std=c11 memory/mem_free.c
	gcc -c -std=c11 memory/mem_realloc.c
	gcc -c -std=c11 parse.c
	gcc -c -std=c11 lex.c
	gcc -c -std=c11 gen.c
	gcc -c -std=c11 system/file.c
	gcc -c -std=c11 system/vec.c
	gcc -c -std=c11 data/mem_cpy.c
	gcc -c -std=c11 data/mem_swp.c
	gcc -c -std=c11 system/flags.c
	nasm -f elf64 -o mutex.o system/asm/mutex.asm
	gcc -c -std=c11 data/str_len.c
	gcc -c -std=c11 data/mem_dupe.c
	gcc -c -std=c11 system/errno.c
	gcc -c -std=c11 data/mem_set.c
	gcc -c -std=c11 data/str_cmp.c
	gcc -c -std=c11 system/buff.c
	gcc -c -std=c11 data/str_dupe.c
	gcc -c -std=c11 data/mem_cmp.c
	gcc -c -std=c11 system/string.c
	nasm -f elf64 -o sys_nanosleep.o system/asm/sys_nanosleep.asm
	gcc -std=c11 -o main script.c string.o sys_nanosleep.o mem_cmp.o str_dupe.o buff.o str_cmp.o mem_set.o errno.o mem_dupe.o str_len.o flags.o io.o mutex.o mem_swp.o mem_cpy.o vec.o file.o parse.o lex.o gen.o mem_realloc.o mem_free.o mem_alloc.o
clean:
	rm -f *.o
#all

##	g++ -DARC64 -I../serializer/inc -L../serializer/lib -std=c++11 player_manager.cpp

#all:
#	cd ../; \
#	make src/memory/alloc_pixmap.o ARC=-DARC64; \
#	make src/memory/mem_alloc.o ARC=-DARC64; \
#	make src/memory/mem_free.o ARC=-DARC64; \
#	cd src;
#	g++ -c -DARC64 -std=c++11 uni_manager.cpp
#	g++ -c -DARC64 -std=c++11 chunk_keeper.cpp
#	g++ -c -DARC64 -std=c++11 chunk_manager.cpp
#	g++ -c -DARC64 -std=c++11 chunk_handler.cpp
#	g++ -std=c++11 -DARC64 main.cpp chunk_keeper.o chunk_handler.o uni_manager.o chunk_manager.o memory/alloc_pixmap.o memory/mem_alloc.o memory/mem_free.o -lpthread
