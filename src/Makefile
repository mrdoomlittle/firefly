DEFINES=-I../eint_t/inc -D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
nasm_flags="-I${PWD}/system/asm/"
dst_dir=${PWD}
cc_flags=-O0 -Wall -D__ffly_mscarcity -Wall -fno-builtin -D__ffly_testing -D__ffly_no_task_pool -D__ffly_use_allocr -D__ffly_source -D__fflib
ffly_objs=$(shell export dst_dir=$(dst_dir);export root_dir=${PWD};. ./compile.sh;echo $$ffly_objs)
fs: clean #claen
	gcc $(cc_flags) -Wall -o main fs/m/test.c $(ffly_objs) -nostdlib
bfv: clean
	gcc $(cc_flags) -c bh.c
	gcc $(cc_flags) -c -o bh/tape.o bh/tape.c
	gcc $(cc_flags) -c -o bh/input.o bh/input.c
	gcc $(cc_flags) -c -o bh/output.o bh/output.c
	gcc $(cc_flags) -Wall -o main bfv.c bh/tape.o bh/output.o bh/input.o bh.o $(ffly_objs) -nostdlib
comp: clean
	gcc $(cc_flags) -D__ffly_compactor_bin -Wall -o main compactor.c $(ffly_objs) -nostdlib
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
#	gcc -c extract.c
	gcc $(cc_flags) -c pkg/construct.c
	gcc $(cc_flags) -Wall -o fpk pkg.c construct.o $(ffly_objs) -nostdlib
crypto: clean
	gcc -Wall -o main crypto.c $(ffly_objs)
hatch: clean
	gcc $(cc_flags) -Wall -o hatch hatch.c $(ffly_objs) -nostdlib
typo: clean
	 gcc $(cc_flags) -Wall $(ffly_objs) -nostdlib
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
lex: clean
	gcc $(cc_flags) -c lexer.c
	gcc $(cc_flags) -Wall -o main lexer.o $(ffly_objs) -nostdlib
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
	gcc $(cc_flags) -o terminal db/terminal.c $(ffly_objs) -nostdlib
db: clean
	gcc $(cc_flags) -o main $(ffly_objs) -nostdlib
db_daemon: clean
	gcc $(cc_flags) -c db/client.c
	gcc $(cc_flags) -c db/user.c
	gcc $(cc_flags) -c db/key.c
	gcc $(cc_flags) -c db/slot.c
#	gcc $(cc_flags) -o client db/test.c $(ffly_objs) connect.o db.o user.o key.o -nostdlib
	gcc $(cc_flags) -o daemon db/daemon.c $(ffly_objs) client.o user.o key.o slot.o -nostdlib
net: clean
#	gcc $(cc_flags) -Wall -c network/sock.c
#	gcc $(cc_flags) -Wall -c network/input.c
#	gcc $(cc_flags) -Wall -c network/output.c
#	gcc $(cc_flags) -Wall -c net.c
	gcc $(cc_flags) -Wall -o client client.c $(ffly_objs) -nostdlib
	gcc $(cc_flags) -Wall -o server server.c $(ffly_objs) -nostdlib
ca: clean
	gcc -c memory/mem_alloc.c
	gcc -c memory/mem_free.c
	gcc -c memory/mem_realloc.c
	gcc cache.c mem_alloc.o mem_free.o mem_realloc.o
hs: clean
	gcc $(cc_flags) -o ffhs $(ffly_objs) -nostdlib
ff: clean
	gcc $(cc_flags) -c -o shell/eval.o shell/eval.c
	gcc $(cc_flags) -c -o shell/parser.o shell/parser.c
	gcc $(cc_flags) -c -o shell/lexer.o shell/lexer.c
	gcc $(cc_flags) -c -o shell/input.o shell/input.c
	gcc $(cc_flags) -c -o shell/memalloc.o shell/memalloc.c
	gcc $(cc_flags) -c -o shell/exec.o shell/exec.c
	gcc $(cc_flags) -c -o shell/hash.o shell/hash.c
	gcc $(cc_flags) -o ff.exec ff.c $(ffly_objs) shell/eval.o shell/parser.o shell/lexer.o shell/input.o shell/memalloc.o shell/exec.o shell/hash.o -nostdlib
rs: clean
	gcc $(cc_flags) $(ffly_objs) -nostdlib
main: clean
	g++ $(cc_flags) main.c $(ffly_objs) -nostdlib
uni: clean
	gcc $(cc_flags) main.c $(ffly_objs) -nostdlib
panel: clean
	gcc $(cc_flags) $(ffly_objs) -nostdlib
ros: clean
	gcc $(cc_flags) $(ffly_objs) -nostdlib
bh: clean
	gcc $(cc_flags) -c bh.c
	gcc $(cc_flags) -c -o bh/tape.o bh/tape.c
	gcc $(cc_flags) -c -o bh/input.o bh/input.c
	gcc $(cc_flags) -c -o bh/output.o bh/output.c
	gcc $(cc_flags) -o ffbhs bh/server.c bh.o bh/input.o bh/output.o bh/tape.o $(ffly_objs) -nostdlib
	gcc $(cc_flags) -o ffbhc bh/client.c bh.o bh/input.o bh/output.o bh/tape.o $(ffly_objs) -nostdlib
workshop: clean
	gcc $(cc_flags) -c workshop/font_forge.c
	gcc $(cc_flags) workshop.c font_forge.o $(ffly_objs) -nostdlib
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
	gcc $(cc_flags) -Wall -o main $(ffly_objs) -nostdlib
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
	gcc $(cc_flags) -o brew/hash.o -c brew/hash.c
	gcc $(cc_flags) -Wall -o main brew/main.c $(ffly_objs) brew/parser.o brew/hash.o brew/gen.o brew/exec.o brew/brew.o brew/lexer.o -nostdlib
ffc: clean
	gcc $(cc_flags) -c -o ff/parser.o ff/parser.c
	gcc $(cc_flags) -c -o ff/gen.o ff/gen.c
	gcc $(cc_flags) -c -o ff/output/resin.o ff/output/resin.c
	gcc $(cc_flags) -c -o ff/output/amd64.o ff/output/amd64.c
	gcc $(cc_flags) -c parser.c
	gcc $(cc_flags) -c lexer.c
#	gcc $(cc_flags) -c gen.c
#	gcc $(cc_flags) -c data/mem_swp.c
	gcc $(cc_flags) -D__ffc_no_script -c compiler.c
	gcc $(cc_flags) -Wall -o ffc ff-lang.c compiler.o $(ffly_objs) parser.o lexer.o ff/gen.o ff/parser.o ff/output/resin.o ff/output/amd64.o -nostdlib
script: clean
	gcc $(cc_flags) -c -o script/parser.o script/parser.c
	gcc $(cc_flags) -c -o script/gen.o script/gen.c
	gcc $(cc_flags) -c -o script/mm.o script/mm.c
	gcc $(cc_flags) -c parser.c
	gcc $(cc_flags) -c lexer.c
#	gcc $(cc_flags) -c gen.c
	gcc $(cc_flags) -o script/exec.o -c script/exec.c
#	gcc $(cc_flags) -c data/mem_swp.c
	gcc $(cc_flags) -D__ffc_no_ff -c compiler.c
	gcc $(cc_flags) -Wall -o main script.c compiler.o $(ffly_objs) script/exec.o script/mm.o script/parser.o script/gen.o parser.o lexer.o -nostdlib
clean:
#	rm -f main
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
