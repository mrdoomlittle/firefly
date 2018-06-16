root_dir=$(realpath ../)
cc_flags="-std=c99 -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd rdm
gcc $cc_flags -o ffrdm main.c $ffly_objs -nostdlib
