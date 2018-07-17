rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd as
ffly_objs="$ffly_objs remf.o elf.o symbol.o stt.o output.o opcodes/resin_tbl.o opcodes/amd64_tbl.o output/resin.o output/amd64.o as.o hash.o alloca.o parser.o exp.o frag.o"
gcc $cc_flags -c -o opcodes/amd64_tbl.o opcodes/amd64_tbl.c
gcc $cc_flags -c -o opcodes/resin_tbl.o opcodes/resin_tbl.c

gcc $cc_flags -c -o output/resin.o output/resin.c
gcc $cc_flags -c -o output/amd64.o output/amd64.c
gcc $cc_flags -c stt.c
gcc $cc_flags -c frag.c
gcc $cc_flags -c symbol.c
gcc $cc_flags -c alloca.c
gcc $cc_flags -c parser.c
gcc $cc_flags -c exp.c
gcc $cc_flags -c as.c
gcc $cc_flags -c remf.c
gcc $cc_flags -c elf.c
gcc $cc_flags -c hash.c
gcc $cc_flags -c -o output.o output.c
gcc $cc_flags -o ffas main.c $ffly_objs -nostdlib
