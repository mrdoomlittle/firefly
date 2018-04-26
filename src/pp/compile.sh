rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr -D__ffly_source -D__fflib"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd pp
ffly_objs="$ffly_objs lexer.o pp.o"
gcc $cc_flags -c pp.c
gcc $cc_flags -c lexer.c
gcc $cc_flags -o ffpp main.c $ffly_objs -nostdlib
