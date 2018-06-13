rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd bond
ffly_objs="$ffly_objs"
gcc $cc_flags -c hash.c
gcc $cc_flags -c bond.c
gcc $cc_flags -c output.c
gcc $cc_flags -o ffbond main.c output.o hash.o bond.o $ffly_objs -nostdlib
