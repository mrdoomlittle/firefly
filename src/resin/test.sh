rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr -D__ffly_source -D__fflib"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd resin
ffly_objs="$ffly_objs"
gcc $cc_flags -o test test.c $ffly_objs -nostdlib
