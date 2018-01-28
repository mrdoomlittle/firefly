DEFINES=-I../eint_t/inc -D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
nasm_flags="-I${PWD}/system/asm/"
ffly_cc=gcc
dst_dir=${PWD}
ffly_objs=$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=system;. ./system/compile.sh;echo $$ffly_objs) \
$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=memory;. ./memory/compile.sh;echo $$ffly_objs) \
$(shell export ffly_cc=$(ffly_cc);export dst_dir=$(dst_dir);export root_dir=data;. ./data/compile.sh;echo $$ffly_objs)
crypto: clean
	gcc -Wall -o main crypto.c $(ffly_objs)
hatch: clean
	gcc -c firefly.c
	gcc -Wall -o hatch hatch.c $(ffly_objs) firefly.o
terminal: clean
	gcc -c firefly.c
	gcc -Wall -o terminal terminal.c $(ffly_objs) firefly.o
db: clean
	gcc -Wall -c network/sock.c
	gcc -c network/input.c
	gcc -c network/output.c
	gcc -Wall -c net.c
	gcc -c db.c
#	gcc -Wall -o main db.c $(ffly_objs) input.o output.o sock.o net.o
	gcc -Wall -c db/user.c
	gcc -Wall -c db/key.c
	gcc -Wall -o connect db/connect.c $(ffly_objs) input.o output.o sock.o net.o db.o user.o key.o
	gcc -Wall -o daemon db/daemon.c $(ffly_objs) input.o output.o sock.o net.o db.o user.o key.o
#	gcc -Wall -o main db/key.c $(ffly_objs)
net: clean
	gcc -c system/io.c
	gcc -c data/bcopy.c
	gcc -c data/bzero.c
	gcc -c data/str_cmp.c
	gcc -c data/str_cpy.c
	gcc -c data/str_dupe.c
	gcc -c data/str_len.c
	gcc -c data/mem_dupe.c
	gcc -c data/mem_cmp.c
	gcc -c system/errno.c
	gcc -Wall -c network/sock.c
	gcc -c network/input.c
	gcc -c network/output.c
	gcc -D__ffly_no_task_pool -c data/mem_set.c
	gcc -D__ffly_no_task_pool -c data/mem_cpy.c
	gcc -Wall -c net.c
	gcc -Wall -o client client.c $(ffly_objs) input.o output.o sock.o errno.o mem_cmp.o mem_cpy.o mem_set.o mem_dupe.o str_len.o str_cpy.o str_dupe.o str_cmp.o bcopy.o bzero.o io.o  net.o
	gcc -Wall -o server server.c $(ffly_objs) input.o output.o sock.o errno.o mem_cmp.o mem_cpy.o mem_set.o mem_dupe.o str_len.o str_cpy.o str_dupe.o str_cmp.o bcopy.o bzero.o io.o net.o
ca: clean
	gcc -c memory/mem_alloc.c
	gcc -c memory/mem_free.c
	gcc -c memory/mem_realloc.c
	gcc cache.c mem_alloc.o mem_free.o mem_realloc.o
uni: clean
	gcc -D__ffly_testing -c cache.c
	gcc -D__ffly_testing -c lot.c
	gcc -D__ffly_testing -c chunk_manager.c
	gcc -D__ffly_testing -c memory/alloc_pixelmap.c
	gcc -D__ffly_testing -c uni.c
	gcc -D__ffly_testing -c chunk.c
	gcc -D__ffly_testing -c obj.c
	gcc -D__ffly_testing -c obj_pool.c
	gcc -D__ffly_testing -c obj_manager.c
	gcc -D__ffly_testing -c ui/camera.c
	gcc -D__ffly_testing -c maths/min.c
	gcc -D__ffly_testing -c maths/max.c
	gcc -D__ffly_testing -c maths/barycentric.c
	gcc -D__ffly_testing -c polygon.c
	gcc -D__ffly_testing -c maths/rotate.c
	nasm -f elf64 $(nasm_flags) -o round.o.0 maths/asm/round.asm
	gcc -D__ffly_testing -c -o round.o.1 maths/round.c

	gcc -D__ffly_testing main.c cache.o round.o.0 round.o.1 rotate.o polygon.o max.o barycentric.o min.o lot.o obj_manager.o obj_pool.o camera.o obj.o alloc_pixelmap.o uni.o chunk.o chunk_manager.o $(ffly_objs) -lm
all:
	rm -f *.o
	nasm -f elf64 $(nasm_flags) -o mutex.o system/asm/mutex.asm
	gcc -c -DARC64 $(DEFINES) maths/is_inside.c
	gcc -c -DARC64 $(DEFINES) system/vec.c
	gcc -c -DARC64 $(DEFINES) system/mem_tracker.c
	gcc -c -DARC64 $(DEFINES) memory/alloc_pixmap.c
	gcc -c -DARC64 $(DEFINES) data/mem_cpy.c
	gcc -c -DARC64 $(DEFINES) data/swp.c
	gcc -c -DARC64 $(DEFINES) system/io.c
	gcc -c -DARC64 $(DEFINES) memory/mem_alloc.c
	gcc -c -DARC64 $(DEFINES) memory/mem_free.c
	gcc -c -DARC64 $(DEFINES) memory/mem_realloc.c
	gcc -c -DARC64 $(DEFINES) -o chunk_keeper.o chunk_keeper.c
	g++ -c -std=c++11 -DARC64 $(DEFINES) -o chunk_manager.o chunk_manager.cpp
	gcc -c -DARC64 $(DEFINES) data/mem_cmp.c
	gcc -c -DARC64 $(DEFINES) data/find.c
	g++ -std=c++11 -DARC64 $(DEFINES) -o main main.cpp mutex.o is_inside.o vec.o find.o mem_tracker.o chunk_manager.o mem_cmp.o swp.o alloc_pixmap.o mem_cpy.o io.o chunk_keeper.o mem_alloc.o mem_free.o mem_realloc.o
config: clean
	gcc -c system/io.c
	gcc -c system/vec.c
	gcc -D__ffly_no_task_pool -c data/mem_cpy.c
	gcc -c data/mem_swp.c
	gcc -c system/flags.c
	nasm -f elf64 $(nasm_flags) -o mutex.o.0 system/asm/mutex.asm
	gcc -c data/str_len.c
	gcc -c data/str_cpy.c
	gcc -c data/mem_dupe.c
	gcc -c system/errno.c
	gcc -D__ffly_no_task_pool -c data/mem_set.c
	gcc -c data/str_cmp.c
	gcc -c system/buff.c
	gcc -c data/str_dupe.c
	gcc -c data/mem_cmp.c
	gcc -c system/bin_tree.c
	gcc -c -o mutex.o.1 system/mutex.c
	gcc -c system/map.c
	gcc -c system/util/hash.c
	nasm -f elf64 $(nasm_flags) -o sys_nanosleep.o system/asm/sys_nanosleep.asm
	gcc  -o main config.c str_cpy.o bin_tree.o hash.o map.o $(ffly_objs) sys_nanosleep.o mem_cmp.o str_dupe.o buff.o str_cmp.o mem_set.o errno.o mem_dupe.o str_len.o flags.o io.o mutex.o.0 mutex.o.1 mem_swp.o mem_cpy.o vec.o
script: clean
	gcc -c parser.c
	gcc -c lexer.c
	gcc -c gen.c
	gcc -c system/file.c
	gcc -c data/mem_swp.c
	gcc  -Wall -o main script.c $(ffly_objs) mem_swp.o parser.o lexer.o gen.o
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
