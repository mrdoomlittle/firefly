rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr -D__ffly_source -D__fflib"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd as
ffly_objs="$ffly_objs ffef.o symbol.o stt.o output.o resin.o as.o hash.o alloca.o parser.o exp.o"
gcc $cc_flags -c output/resin.c
gcc $cc_flags -c stt.c
gcc $cc_flags -c symbol.c
gcc $cc_flags -c alloca.c
gcc $cc_flags -c parser.c
gcc $cc_flags -c exp.c
gcc $cc_flags -c as.c
gcc $cc_flags -c ffef.c
gcc $cc_flags -c hash.c
gcc $cc_flags -c -o output.o output.c
gcc $cc_flags -o ffas main.c $ffly_objs -nostdlib
