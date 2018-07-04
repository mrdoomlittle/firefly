rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd mat
ffly_objs="$ffly_objs mat.o"
gcc $cc_flags -c -o mat.o ../mat.c
gcc $cc_flags -o ffmat main.c $ffly_objs -nostdlib
