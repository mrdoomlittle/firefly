rm -f lib/libmalloc.so src/malloc.o
gcc -c -fPIC -o src/malloc.o src/malloc.c
gcc -shared -o lib/libmalloc.so src/malloc.o
