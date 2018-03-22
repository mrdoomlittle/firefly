rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd exec
ffly_objs="$ffly_objs"
gcc $cc_flags -o exec main.c $ffly_objs -nostdlib
