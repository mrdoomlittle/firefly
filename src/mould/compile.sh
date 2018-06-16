rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd mould
ffly_objs="$ffly_objs mould.o exec.o gen.o parser.o lexer.o hash.o job.o"
if ! [ -f ffmould ]; then
gcc $cc_flags -o mould.o -c mould.c
gcc $cc_flags -o exec.o -c exec.c
gcc $cc_flags -o gen.o -c gen.c
gcc $cc_flags -o parser.o -c parser.c
gcc $cc_flags -o lexer.o -c lexer.c
gcc $cc_flags -o hash.o -c hash.c
gcc $cc_flags -o job.o -c job.c
else
./ffmould compile.mould -s "cc=/usr/bin/gcc" -s "cc_flags=$cc_flags"
fi
gcc $cc_flags -o ffmould main.c $ffly_objs -nostdlib
