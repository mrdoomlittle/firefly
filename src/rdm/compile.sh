root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd rdm
gcc $cc_flags -o ffrdm main.c $ffly_objs -nostdlib
