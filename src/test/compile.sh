gcc -c -std=gnu99 ../system/string.c
gcc -c -std=gnu99 ../drm.c
gcc -std=gnu99 -o main main.c drm.o string.o -ldrm
