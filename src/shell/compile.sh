rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd shell
ffly_objs="$ffly_objs eval.o parser.o lexer.o input.o memalloc.o exec.o hash.o"
gcc $cc_flags -c eval.c
gcc $cc_flags -c parser.c
gcc $cc_flags -c lexer.c
gcc $cc_flags -c input.c
gcc $cc_flags -c memalloc.c
gcc $cc_flags -c exec.c
gcc $cc_flags -c hash.c
gcc $cc_flags main.c $ffly_objs -nostdlib
