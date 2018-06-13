rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd brew
ffly_objs="$ffly_objs brew.o exec.o gen.o parser.o lexer.o hash.o"
gcc $cc_flags -o brew.o -c brew.c
gcc $cc_flags -o exec.o -c exec.c
gcc $cc_flags -o gen.o -c gen.c
gcc $cc_flags -o parser.o -c parser.c
gcc $cc_flags -o lexer.o -c lexer.c
gcc $cc_flags -o hash.o -c hash.c

gcc $cc_flags -o ffbrew main.c $ffly_objs -nostdlib
