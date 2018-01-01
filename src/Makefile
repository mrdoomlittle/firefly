DEFINES=-I../eint_t/inc -D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
nasm_flags="-I${PWD}/system/asm/"
uni: clean
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 system/asm/mutex.asm
	gcc -c -o mutex.o.1 system/mutex.c
	gcc -c memory/mem_alloc.c
	gcc -c memory/mem_free.c
	gcc -c memory/mem_realloc.c
	gcc -c chunk_manager.c
	gcc -c memory/alloc_pixelmap.c
	gcc -c uni.c
	gcc -c chunk.c
	gcc -c system/io.c
	gcc -c obj.c
	gcc -c system/vec.c
	gcc -c data/mem_cpy.c
	gcc -c system/flags.c
	gcc -c ui/camera.c
	gcc -c data/mem_set.c
	nasm -f elf64 $(nasm_flags) -o sys_nanosleep.o.0 system/asm/sys_nanosleep.asm
	gcc -c -o sys_nanosleep.o.1 system/sys_nanosleep.c
	gcc main.c mutex.o.0 mem_set.o sys_nanosleep.o.0 sys_nanosleep.o.1 camera.o obj.o mutex.o.1 flags.o vec.o mem_cpy.o alloc_pixelmap.o uni.o io.o chunk.o chunk_manager.o mem_alloc.o mem_realloc.o mem_free.o

all:
	rm -f *.o
	nasm -f elf64 $(nasm_flags) -o mutex.o system/asm/mutex.asm
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
config: clean
	gcc -c -std=c11 system/io.c
	gcc -c -std=c11 memory/mem_alloc.c
	gcc -c -std=c11 memory/mem_free.c
	gcc -c -std=c11 memory/mem_realloc.c
	gcc -c -std=c11 system/file.c
	gcc -c -std=c11 system/vec.c
	gcc -c -std=c11 data/mem_cpy.c
	gcc -c -std=c11 data/mem_swp.c
	gcc -c -std=c11 system/flags.c
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 system/asm/mutex.asm
	gcc -c -std=c11 data/str_len.c
	gcc -c -std=c11 data/mem_dupe.c
	gcc -c -std=c11 system/errno.c
	gcc -c -std=c11 data/mem_set.c
	gcc -c -std=c11 data/str_cmp.c
	gcc -c -std=c11 system/buff.c
	gcc -c -std=c11 data/str_dupe.c
	gcc -c -std=c11 data/mem_cmp.c
	gcc -c -std=c11 system/string.c
	gcc -c -std=c11 system/bin_tree.c
	gcc -c -std=c11 -o mutex.o.1 system/mutex.c
	gcc -c -std=c11 system/map.c
	gcc -c -std=c11 system/util/hash.c
	nasm -f elf64 $(nasm_flags) -o sys_nanosleep.o system/asm/sys_nanosleep.asm
	gcc -std=c11 -o main config.c bin_tree.o hash.o map.o string.o sys_nanosleep.o mem_cmp.o str_dupe.o buff.o str_cmp.o mem_set.o errno.o mem_dupe.o str_len.o flags.o io.o mutex.o.0 mutex.o.1 mem_swp.o mem_cpy.o vec.o file.o mem_realloc.o mem_free.o mem_alloc.o
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
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 system/asm/mutex.asm
	gcc -c -std=c11 data/str_len.c
	gcc -c -std=c11 data/mem_dupe.c
	gcc -c -std=c11 system/errno.c
	gcc -c -std=c11 data/mem_set.c
	gcc -c -std=c11 data/str_cmp.c
	gcc -c -std=c11 system/buff.c
	gcc -c -std=c11 data/str_dupe.c
	gcc -c -std=c11 data/mem_cmp.c
	gcc -c -std=c11 system/string.c
	gcc -c -std=c11 system/bin_tree.c
	gcc -c -std=c11 -o mutex.o.1 system/mutex.c
	gcc -c -std=c11 system/map.c
	gcc -c -std=c11 system/util/hash.c
	nasm -f elf64 $(nasm_flags) -o sys_nanosleep.o system/asm/sys_nanosleep.asm
	gcc -std=c11 -Wall -o main script.c bin_tree.o hash.o map.o string.o sys_nanosleep.o mem_cmp.o str_dupe.o buff.o str_cmp.o mem_set.o errno.o mem_dupe.o str_len.o flags.o io.o mutex.o.0 mutex.o.1 mem_swp.o mem_cpy.o vec.o file.o parse.o lex.o gen.o mem_realloc.o mem_free.o mem_alloc.o
clean:
	rm -f main
	cd ../ && make clean && cd src
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
