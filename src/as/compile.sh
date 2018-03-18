rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd as
ffly_objs="$ffly_objs ffef.o output.o bc.o x86.o as.o hash.o alloca.o parser.o exp.o"
gcc $cc_flags -c output/x86.c
gcc $cc_flags -c output/bc.c
gcc $cc_flags -c alloca.c
gcc $cc_flags -c parser.c
gcc $cc_flags -c exp.c
gcc $cc_flags -c as.c
gcc $cc_flags -c ffef.c
gcc $cc_flags -c hash.c
gcc $cc_flags -c -o output.o output.c
gcc $cc_flags -o as main.c $ffly_objs -nostdlib
