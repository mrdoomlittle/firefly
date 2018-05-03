DEFINES=-I../eint_t/inc -D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
nasm_flags="-I${PWD}/system/asm/"
dst_dir=${PWD}
cc_flags=-O0 -Wall -fno-builtin -D__ffly_testing -D__ffly_no_task_pool -D__ffly_use_allocr -D__ffly_source -D__fflib
ffly_objs=$(shell export dst_dir=$(dst_dir);export root_dir=${PWD};. ./compile.sh;echo $$ffly_objs)
lexer: clean
	gcc $(cc_flags) -Wall -o main lexer.c $(ffly_objs) -nostdlib
test: clean
	gcc $(cc_flags) -Wall -o main test.c $(ffly_objs) -nostdlib
mod: clean
	gcc $(cc_flags) -c -Wall mod/pipe.c -o mod_pipe.o
#	gcc $(cc_flags) -c -Wall -o uapi/mod/printf.o uapi/mod/printf.c
#	gcc $(cc_flags) -c -Wall -o uapi/mod/malloc.o uapi/mod/malloc.c
	gcc $(cc_flags) -c -Wall call.c
	gcc $(cc_flags) -Wall -o main mod.c mod_pipe.o call.o $(ffly_objs) -nostdlib
pkg: clean
	gcc -c extract.c
	gcc -c build.c
	gcc -Wall -o main pkg.c build.o extract.o $(ffly_objs)
crypto: clean
	gcc -Wall -o main crypto.c $(ffly_objs)
hatch: clean
	gcc $(cc_flags) -Wall -o hatch hatch.c $(ffly_objs) -nostdlib
terminal: clean
	gcc $(cc_flags) -Wall -o terminal terminal.c $(ffly_objs) -nostdlib
mat: clean
	gcc $(cc_flags) -c mat.c
	gcc $(cc_flags) -c mtp.c
	gcc $(cc_flags) -c mat/daemon.c
	gcc $(cc_flags) -c mat/client.c
	gcc $(cc_flags) -Wall -o daemon mat.o mtp.o daemon.o $(ffly_objs) -nostdlib
	gcc $(cc_flags) -Wall -o client mat.o mtp.o client.o $(ffly_objs) -nostdlib
#	gcc $(cc_flags) -Wall -o main mat.o mat/main.c $(ffly_objs) -nostdlib
mul: clean
	gcc $(cc_flags) -c mul/hash.c
	gcc $(cc_flags) -c mul/lexer.c
	gcc $(cc_flags) -c mul/mul.c
	gcc $(cc_flags) -Wall -o main mul/main.c mul.o hash.o lexer.o $(ffly_objs) -nostdlib
#db: clean
#	gcc -Wall -c network/sock.c
#	gcc -c network/input.c
#	gcc -c network/output.c
#	gcc -Wall -c net.c
#	gcc -c db.c
#	gcc -c crypto.c
##	gcc -Wall -o main db.c $(ffly_objs) input.o output.o sock.o net.o
#	gcc -Wall -c db/user.c
#	gcc -Wall -c db/key.c
#	gcc -Wall -o connect crypto.o db/connect.c $(ffly_objs) input.o output.o sock.o net.o db.o user.o key.o
#	gcc -Wall -o daemon crypto.o db/daemon.c $(ffly_objs) input.o output.o sock.o net.o db.o user.o key.o
##	gcc -Wall -o main db/key.c $(ffly_objs)
db_term: clean
	gcc $(cc_flags) -c db.c
	gcc $(cc_flags) -c db/connect.c
	gcc $(cc_flags) -o terminal db/terminal.c db.o connect.o $(ffly_objs) -nostdlib
db: clean
	gcc $(cc_flags) -c db.c #$(ffly_objs) -nostdlib
	gcc $(cc_flags) -c db/client.c
	gcc $(cc_flags) -c db/user.c
	gcc $(cc_flags) -c db/key.c
	gcc $(cc_flags) -c db/slot.c
	gcc $(cc_flags) -c db/connect.c
	gcc $(cc_flags) -o client db/test.c $(ffly_objs) connect.o db.o user.o key.o -nostdlib
	gcc $(cc_flags) -o daemon db/daemon.c $(ffly_objs) client.o db.o user.o key.o slot.o -nostdlib
net: clean
	gcc -Wall -c network/sock.c
	gcc -Wall -c network/input.c
	gcc -Wall -c network/output.c
	gcc -Wall -c net.c
	gcc -Wall -o client client.c $(ffly_objs) net.o input.o output.o sock.o
	gcc -Wall -o server server.c $(ffly_objs) net.o input.o output.o sock.o
ca: clean
	gcc -c memory/mem_alloc.c
	gcc -c memory/mem_free.c
	gcc -c memory/mem_realloc.c
	gcc cache.c mem_alloc.o mem_free.o mem_realloc.o
main: clean
	g++ $(cc_flags) main.c $(ffly_objs) -nostdlib
uni: clean
	gcc $(cc_flags) main.c $(ffly_objs) -nostdlib
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
arcs: clean
	gcc $(cc_flags) -Wall -o main $(ffly_objs) -nostdlib
bci: clean
	gcc $(cc_flags) -Wall -o main bci/main.c $(ffly_objs) -nostdlib
brew: clean
	gcc $(cc_flags) -o brew/brew.o -c brew/brew.c
	gcc $(cc_flags) -o brew/exec.o -c brew/exec.c
	gcc $(cc_flags) -o brew/gen.o -c brew/gen.c
	gcc $(cc_flags) -o brew/parser.o -c brew/parser.c
	gcc $(cc_flags) -o brew/lexer.o -c brew/lexer.c
	gcc $(cc_flags) -Wall -o main brew/main.c $(ffly_objs) brew/parser.o brew/gen.o brew/exec.o brew/brew.o brew/lexer.o -nostdlib
ffc: clean
	gcc $(cc_flags) -c -o ff/parser.o ff/parser.c
	gcc $(cc_flags) -c -o ff/gen.o ff/gen.c
	gcc $(cc_flags) -c parser.c
	gcc $(cc_flags) -c lexer.c
#	gcc $(cc_flags) -c gen.c
#	gcc $(cc_flags) -c data/mem_swp.c
	gcc $(cc_flags) -D__ffc_no_script -c compiler.c
	gcc $(cc_flags) -Wall -o ffc ff-lang.c compiler.o $(ffly_objs) parser.o lexer.o ff/parser.o ff/gen.o -nostdlib
script: clean
	gcc $(cc_flags) -c -o script/parser.o script/parser.c
	gcc $(cc_flags) -c -o script/gen.o script/gen.c
	gcc $(cc_flags) -c parser.c
	gcc $(cc_flags) -c lexer.c
#	gcc $(cc_flags) -c gen.c
	gcc $(cc_flags) -o script/exec.o -c script/exec.c
#	gcc $(cc_flags) -c data/mem_swp.c
	gcc $(cc_flags) -D__ffc_no_ff -c compiler.c
	gcc $(cc_flags) -Wall -o main script.c compiler.o $(ffly_objs) script/exec.o script/parser.o script/gen.o parser.o lexer.o -nostdlib
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
