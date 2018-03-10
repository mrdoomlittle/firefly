cc_flags="-D__ffly_module -fno-builtin"

gcc $cc_flags -c main.c
gcc $cc_flags -c ../src/mod/printf.c
gcc $cc_flags -c ../src/mod/malloc.c
gcc $cc_flags -c ../src/system/string.c
gcc $cc_flags -c ../src/system/shm.c
gcc $cc_flags -c ../src/system/pipe.c
gcc $cc_flags -c ../src/mod/io.c
gcc $cc_flags -c ../src/mod/ring.c
gcc $cc_flags -c -o mod_pipe.o ../src/mod/pipe.c

as_inc="-I $(realpath ../)/src/system/asm/"
nasm -f elf64 $as_inc -o sys_shmctl.o.0 ../src/system/asm/sys_shmctl.asm
gcc $cc_flags -c -o sys_shmctl.o.1 ../src/system/sys_shmctl.c

nasm -f elf64 $as_inc -o sys_shmget.o.0 ../src/system/asm/sys_shmget.asm
gcc $cc_flags -c -o sys_shmget.o.1 ../src/system/sys_shmget.c

nasm -f elf64 $as_inc -o sys_shmat.o.0 ../src/system/asm/sys_shmat.asm
gcc $cc_flags -c -o sys_shmat.o.1 ../src/system/sys_shmat.c

nasm -f elf64 $as_inc -o sys_shmdt.o.0 ../src/system/asm/sys_shmdt.asm
gcc $cc_flags -c -o sys_shmdt.o.1 ../src/system/sys_shmdt.c

gcc $cc_flags -c -o bcopy.o.1 ../src/dep/bcopy.c
as -o bcopy.o.0 ../src/dep/bcopy.s
gcc $cc_flags main.o $ffly_objs pipe.o shm.o string.o malloc.o printf.o bcopy.o.1 bcopy.o.0 \
sys_shmctl.o.0 sys_shmctl.o.1 sys_shmget.o.0 sys_shmget.o.1 sys_shmat.o.0 sys_shmat.o.1 \
sys_shmdt.o.0 sys_shmdt.o.1 io.o ring.o mod_pipe.o $1 -nostdlib
