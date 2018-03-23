rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd bond
ffly_objs="$ffly_objs"
gcc $cc_flags -c hash.c
gcc $cc_flags -c bond.c
gcc $cc_flags -o bond main.c hash.o bond.o $ffly_objs -nostdlib
