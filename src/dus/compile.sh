rm -f *.o
root_dir=$(realpath ../)
cc_flags="-std=c99 -D__ffly_crucial"
dst_dir=$root_dir
cd ../ && . ./compile.sh && cd dus
ffly_objs="$ffly_objs dus.o exec.o exp.o lexer.o parser.o gen.o hash.o"
if ! [ -f ffdus ]; then
	gcc $cc_flags -c -o dus.o dus.c
	gcc $cc_flags -c -o exec.o exec.c
	gcc $cc_flags -c -o exp.o exp.c
	gcc $cc_flags -c -o lexer.o lexer.c
	gcc $cc_flags -c -o parser.o parser.c
	gcc	$cc_flags -c -o gen.o gen.c
	gcc $cc_flags -c -o hash.o hash.c
else
	./ffdus compile.dus "/usr/bin/gcc" "$cc_flags"
fi
gcc $cc_flags -o ffdus main.c $ffly_objs -nostdlib
