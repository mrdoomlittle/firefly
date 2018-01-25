#!/bin/sh
$ffly_cc -c -o $dst_dir/bin_tree.o $root_dir/bin_tree.c
$ffly_cc -c -o $dst_dir/buff.o $root_dir/buff.c
$ffly_cc -c -o $dst_dir/vec.o $root_dir/vec.c
$ffly_cc -c -o $dst_dir/map.o $root_dir/map.c
$ffly_cc -c -o $dst_dir/flags.o $root_dir/flags.c
$ffly_cc -c -o $dst_dir/hash.o $root_dir/util/hash.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/atomic_op.o.0 $root_dir/asm/atomic_op.asm
$ffly_cc -c -o $dst_dir/atomic_op.o.1 $root_dir/atomic_op.c

$ffly_cc -c -o $dst_dir/pipe.o $root_dir/pipe.c
$ffly_cc -c -o $dst_dir/shm.o $root_dir/shm.c
$ffly_cc -c -o $dst_dir/errno.o $root_dir/errno.c
$ffly_cc -c -o $dst_dir/io.o $root_dir/io.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_nanosleep.o.0 $root_dir/asm/sys_nanosleep.asm
$ffly_cc -c -o $dst_dir/sys_nanosleep.o.1 $root_dir/sys_nanosleep.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/mutex.o.0 $root_dir/asm/mutex.asm
$ffly_cc -c -o $dst_dir/mutex.o.1 $root_dir/mutex.c

nasm -f elf64 -o $dst_dir/sys_fcntl.o.0 $root_dir/asm/sys_fcntl.asm
$ffly_cc -c -o $dst_dir/sys_fcntl.o.1 $root_dir/sys_fcntl.c

nasm -f elf64 -o $dst_dir/sys_open.o.0 $root_dir/asm/sys_open.asm
$ffly_cc -c -o $dst_dir/sys_open.o.1 $root_dir/sys_open.c

nasm -f elf64 -o $dst_dir/sys_close.o.0 $root_dir/asm/sys_close.asm
$ffly_cc -c -o $dst_dir/sys_close.o.1 $root_dir/sys_close.c

nasm -f elf64 -o $dst_dir/sys_write.o.0 $root_dir/asm/sys_write.asm
$ffly_cc -c -o $dst_dir/sys_write.o.1 $root_dir/sys_write.c

nasm -f elf64 -o $dst_dir/sys_brk.o.0 $root_dir/asm/sys_brk.asm
gcc -c -o $dst_dir/sys_brk.o.1 $root_dir/sys_brk.c

$ffly_cc -c -o $dst_dir/printf.o $root_dir/printf.c
$ffly_cc -c -o $dst_dir/string.o $root_dir/string.c
$ffly_cc -c -o $dst_dir/file.o $root_dir/file.c
export ffly_objs="$dst_dir/sys_fcntl.o.0 $dst_dir/sys_fcntl.o.1 \
$dst_dir/sys_open.o.0 $dst_dir/sys_open.o.1 \
$dst_dir/sys_close.o.0 $dst_dir/sys_close.o.1 \
$dst_dir/sys_write.o.0 $dst_dir/sys_write.o.1 \
$dst_dir/printf.o $dst_dir/string.o $dst_dir/file.o \
$dst_dir/sys_brk.o.0 $dst_dir/sys_brk.o.1 \
$dst_dir/mutex.o.0 $dst_dir/mutex.o.1 \
$dst_dir/errno.o $dst_dir/io.o $dst_dir/pipe.o $dst_dir/shm.o \
$dst_dir/atomic_op.o.0 $dst_dir/atomic_op.o.1 \
$dst_dir/vec.o $dst_dir/map.o $dst_dir/flags.o $dst_dir/hash.o \
$dst_dir/buff.o $dst_dir/bin_tree.o \
$dst_dir/sys_nanosleep.o.0 $dst_dir/sys_nanosleep.o.1"
