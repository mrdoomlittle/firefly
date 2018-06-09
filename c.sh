gcc -c src/system/util/ff6.c
gcc -c src/system/string.c
gcc version_gen.c string.o ff6.o -o version_gen
