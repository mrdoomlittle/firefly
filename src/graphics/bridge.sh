use_x11=0
use_xcb=0
use_slurry=1
cc_flags="-Wall -D__ffly_has_libpng -D__ffly_has_libjpeg -D__ffly_bridge -D__ffly_source"
rm -f *.o
gcc $cc_flags -c ../system/file.c
gcc $cc_flags -c ../system/io.c
gcc $cc_flags -c ../dep/mem_dup.c
gcc $cc_flags -c ../dep/mem_set.c
gcc $cc_flags -c ../dep/str_len.c
gcc $cc_flags -c ../dep/mem_cpy.c
gcc $cc_flags -c ../dep/bcopy.c
if [ $use_x11 -eq 1 ]; then
	cc_flags="$cc_flags -D__ffly_use_x11"
	gcc $cc_flags -c x11.c
	gcc $cc_flags -c x11_m.c
fi

if [ $use_xcb -eq 1 ]; then
	cc_flags="$cc_flags -D__ffly_use_xcb"
	gcc $cc_flags -c xcb.c
	gcc $cc_flags -c xcb_m.c
fi

if [ $use_slurry -eq 1 ]; then
	cc_flags="$cc_flags -D__ffly_use_slurry"
	gcc $cc_flags -c ../slurry/connection.c
	gcc $cc_flags -c ../slurry/client.c
	gcc $cc_flags -c ../slurry/slurry.c
	gcc $cc_flags -c ../slurry/proto.c
	gcc $cc_flags -c slurry_m.c
fi

gcc $cc_flags -c window.c
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
gcc $cc_flags -c mare.c
gcc $cc_flags -c ../oddity.c
gcc $cc_flags -c ../duct/shm_pipe.c
gcc $cc_flags -c ../log.c
gcc $cc_flags -c ../system/posix_fs.c
gcc $cc_flags -c ../system/fs.c
objs="window.o io.o file.o mem_dup.o mem_set.o str_len.o mem_cpy.o \
mutex.o.0 mutex.o.1 atomic_op.o.0 atomic_op.o.1 \
mem_alloc.o mem_free.o mem_realloc.o pool.o event.o.0 event.o.1 queue.o \
pipe.o shm.o mode.o bcopy.o oddity.o fs.o duct.o image.o png.o jpeg.o sys_nanosleep.o.0 sys_nanosleep.o.1 mare.o shm_pipe.o log.o posix_fs.o"
if [ $use_x11 -eq 1 ]; then
	objs="$objs x11.o x11_m.o"
fi
if [ $use_xcb -eq 1 ]; then
	objs="$objs xcb.o xcb_m.o" 
fi
if [ $use_slurry -eq 1 ]; then
	objs="$objs slurry_m.o client.o connection.o slurry.o proto.o"
fi
gcc $cc_flags bridge.c $objs -lpng -ljpeg -lX11 -lGL -lglut -lX11-xcb -lxcb -lxcb-icccm 
