cc_flags="$3 -D__fflib -D__ffly_source -D__ffly_module -fno-builtin"

gcc $cc_flags -c start.c
gcc $cc_flags -c ../src/mod/printf.c
gcc $cc_flags -c ../src/mod/malloc.c
gcc $cc_flags -c ../src/system/string.c
gcc $cc_flags -c ../src/system/shm.c
gcc $cc_flags -c ../src/system/port.c
gcc $cc_flags -c ../src/mod/io.c
gcc $cc_flags -c ../src/mod/ring.c
gcc $cc_flags -c ../src/mode.c
gcc $cc_flags -c -o mod_port.o ../src/mod/port.c
gcc $cc_flags -c -o copy.o ../src/mod/copy.c

as_inc="-I$(realpath ../)/src/system/asm/ -I$(realpath ../)/src/linux/asm/"
nasm -f elf64 $as_inc -o sys_shmctl.o.0 ../src/system/asm/sys_shmctl.asm
gcc $cc_flags -c -o sys_shmctl.o.1 ../src/system/sys_shmctl.c

nasm -f elf64 $as_inc -o sys_shmget.o.0 ../src/system/asm/sys_shmget.asm
gcc $cc_flags -c -o sys_shmget.o.1 ../src/system/sys_shmget.c

nasm -f elf64 $as_inc -o sys_shmat.o.0 ../src/system/asm/sys_shmat.asm
gcc $cc_flags -c -o sys_shmat.o.1 ../src/system/sys_shmat.c

nasm -f elf64 $as_inc -o sys_shmdt.o.0 ../src/system/asm/sys_shmdt.asm
gcc $cc_flags -c -o sys_shmdt.o.1 ../src/system/sys_shmdt.c

nasm -f elf64 $as_inc -o mutex.o.0 ../src/system/asm/mutex.asm
gcc $cc_flags -c -o mutex.o.1 ../src/system/mutex.c

gcc $cc_flags -c -o bcopy.o.1 ../src/dep/bcopy.c
as -o bcopy.o.0 ../src/dep/bcopy.s

gcc $cc_flags -c -o mem_cpy.o.1 ../src/dep/mem_cpy.c
as -o mem_cpy.o.0 ../src/dep/mem_cpy.s

gcc $cc_flags -c ../src/system/errno.c
ffly_objs="port.o shm.o string.o malloc.o printf.o bcopy.o.1 bcopy.o.0 \
sys_shmctl.o.0 sys_shmctl.o.1 sys_shmget.o.0 sys_shmget.o.1 sys_shmat.o.0 sys_shmat.o.1 \
sys_shmdt.o.0 sys_shmdt.o.1 io.o mode.o ring.o mod_port.o copy.o mutex.o.0 mutex.o.1 \
errno.o mem_cpy.o.0 mem_cpy.o.1"

gcc $cc_flags -o $2 start.o $ffly_objs $1 -nostdlib
