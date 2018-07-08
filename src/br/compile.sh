rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd br
ffly_objs="$ffly_objs"
gcc $cc_flags -o server server.c $ffly_objs -nostdlib
gcc $cc_flags -o client client.c $ffly_objs -nostdlib
