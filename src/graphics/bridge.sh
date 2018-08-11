cc_flags="-D__ffly_has_libpng -D__ffly_has_libjpeg -D__ffly_use_x11 -D__ffly_bridge -D__ffly_source"
rm -f *.o
gcc $cc_flags -c ../system/file.c
gcc $cc_flags -c window.c
gcc $cc_flags -c ../system/io.c
gcc $cc_flags -c ../dep/mem_dup.c
gcc $cc_flags -c ../dep/mem_set.c
gcc $cc_flags -c ../dep/str_len.c
gcc $cc_flags -c ../dep/mem_cpy.c
gcc $cc_flags -c ../dep/bcopy.c
gcc $cc_flags -c x11.c
gcc $cc_flags -c x11_wd.c

#gcc $cc_flags -c xcb.c
#gcc $cc_flags -c xcb_wd.c
as_inc="-I$(realpath ../)/system/asm/"

nasm -f elf64 $as_inc -o mutex.o.0 ../system/asm/mutex.asm
gcc $cc_flags -c -o mutex.o.1 ../system/mutex.c

nasm -f elf64 $as_inc -o atomic_op.o.0 ../system/asm/atomic_op.asm
gcc $cc_flags -c -o atomic_op.o.1 ../system/atomic_op.c

nasm -f elf64 $as_inc -o sys_nanosleep.o.0 ../system/asm/sys_nanosleep.asm
gcc $cc_flags -c -o sys_nanosleep.o.1 ../system/sys_nanosleep.c

gcc $cc_flags -c ../memory/mem_alloc.c
gcc $cc_flags -c ../memory/mem_free.c
gcc $cc_flags -c ../memory/mem_realloc.c

gcc $cc_flags -c ../duct.c
gcc $cc_flags -c ../system/pool.c
gcc $cc_flags -c -o event.o.0 ../system/event.c
gcc $cc_flags -c -o event.o.1 ../event.c
gcc $cc_flags -c ../system/queue.c
gcc $cc_flags -c ../system/pipe.c
gcc $cc_flags -c ../system/shm.c
gcc $cc_flags -c ../mode.c
gcc $cc_flags -c image.c
gcc $cc_flags -c png.c
gcc $cc_flags -c jpeg.c

objs="window.o io.o x11.o x11_wd.o file.o mem_dup.o mem_set.o str_len.o mem_cpy.o \
mutex.o.0 mutex.o.1 atomic_op.o.0 atomic_op.o.1 \
mem_alloc.o mem_free.o mem_realloc.o pool.o event.o.0 event.o.1 queue.o \
pipe.o shm.o mode.o bcopy.o duct.o image.o png.o jpeg.o sys_nanosleep.o.0 sys_nanosleep.o.1"
gcc $cc_flags bridge.c $objs -lpng -ljpeg -lX11 -lGL -lglut -lX11-xcb -lxcb -lxcb-icccm 
