#!/bin/sh
$ffly_cc -c $cc_flags -o $dst_dir/realpath.o $root_dir/realpath.c
$ffly_cc -c $cc_flags -o $dst_dir/err.o $root_dir/err.c
nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/cond_lock.o $root_dir/asm/cond_lock.asm
$ffly_cc -c $cc_flags -o $dst_dir/config.o $root_dir/config.c
$ffly_cc -c $cc_flags -o $dst_dir/thread.o $root_dir/thread.c
#$ffly_cc -c $cc_flags -o $dst_dir/time.o $root_dir/time.c
#$ffly_cc -c $cc_flags -o $dst_dir/task_worker.o $root_dir/task_worker.c
#$ffly_cc -c $cc_flags -o $dst_dir/task.o $root_dir/task.c
#$ffly_cc -c $cc_flags -o $dst_dir/task_pool.o $root_dir/task_pool.c

$ffly_cc -c $cc_flags -o $dst_dir/bin_tree.o $root_dir/bin_tree.c
$ffly_cc -c $cc_flags -o $dst_dir/buff.o $root_dir/buff.c
$ffly_cc -c $cc_flags -o $dst_dir/vec.o $root_dir/vec.c
$ffly_cc -c $cc_flags -o $dst_dir/map.o $root_dir/map.c
$ffly_cc -c $cc_flags -o $dst_dir/flags.o $root_dir/flags.c
$ffly_cc -c $cc_flags -o $dst_dir/hash.o $root_dir/util/hash.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/atomic_op.o.0 $root_dir/asm/atomic_op.asm
$ffly_cc -c $cc_flags -o $dst_dir/atomic_op.o.1 $root_dir/atomic_op.c

$ffly_cc -c $cc_flags -o $dst_dir/pipe.o $root_dir/pipe.c
$ffly_cc -c $cc_flags -o $dst_dir/shm.o $root_dir/shm.c
$ffly_cc -c $cc_flags -o $dst_dir/errno.o $root_dir/errno.c
$ffly_cc -c $cc_flags -o $dst_dir/io.o $root_dir/io.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_nanosleep.o.0 $root_dir/asm/sys_nanosleep.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_nanosleep.o.1 $root_dir/sys_nanosleep.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/mutex.o.0 $root_dir/asm/mutex.asm
$ffly_cc -c $cc_flags -o $dst_dir/mutex.o.1 $root_dir/mutex.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_fcntl.o.0 $root_dir/asm/sys_fcntl.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_fcntl.o.1 $root_dir/sys_fcntl.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_open.o.0 $root_dir/asm/sys_open.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_open.o.1 $root_dir/sys_open.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_close.o.0 $root_dir/asm/sys_close.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_close.o.1 $root_dir/sys_close.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_write.o.0 $root_dir/asm/sys_write.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_write.o.1 $root_dir/sys_write.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_brk.o.0 $root_dir/asm/sys_brk.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_brk.o.1 $root_dir/sys_brk.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_exit.o.0 $root_dir/asm/sys_exit.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_exit.o.1 $root_dir/sys_exit.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_stat.o.0 $root_dir/asm/sys_stat.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_stat.o.1 $root_dir/sys_stat.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_lseek.o.0 $root_dir/asm/sys_lseek.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_lseek.o.1 $root_dir/sys_lseek.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_creat.o.0 $root_dir/asm/sys_creat.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_creat.o.1 $root_dir/sys_creat.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_read.o.0 $root_dir/asm/sys_read.asm
$ffly_cc -c $cc_flags -c -o $dst_dir/sys_read.o.1 $root_dir/sys_read.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_access.o.0 $root_dir/asm/sys_access.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_access.o.1 $root_dir/sys_access.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_fsync.o.0 $root_dir/asm/sys_fsync.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_fsync.o.1 $root_dir/sys_fsync.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_getpid.o.0 $root_dir/asm/sys_getpid.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_getpid.o.1 $root_dir/sys_getpid.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_kill.o.0 $root_dir/asm/sys_kill.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_kill.o.1 $root_dir/sys_kill.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_mmap.o.0 $root_dir/asm/sys_mmap.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_mmap.o.1 $root_dir/sys_mmap.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_munmap.o.0 $root_dir/asm/sys_munmap.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_munmap.o.1 $root_dir/sys_munmap.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_shmctl.o.0 $root_dir/asm/sys_shmctl.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmctl.o.1 $root_dir/sys_shmctl.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_shmget.o.0 $root_dir/asm/sys_shmget.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmget.o.1 $root_dir/sys_shmget.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_shmat.o.0 $root_dir/asm/sys_shmat.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmat.o.1 $root_dir/sys_shmat.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_shmdt.o.0 $root_dir/asm/sys_shmdt.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmdt.o.1 $root_dir/sys_shmdt.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_getcwd.o.0 $root_dir/asm/sys_getcwd.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_getcwd.o.1 $root_dir/sys_getcwd.c

nasm -f elf64 -I$root_dir/asm/ -o $dst_dir/sys_clone.o.0 $root_dir/asm/sys_clone.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_clone.o.1 $root_dir/sys_clone.c

$ffly_cc -c $cc_flags -D__ffly_debug -o $dst_dir/printf.o $root_dir/printf.c
$ffly_cc -c $cc_flags -o $dst_dir/string.o $root_dir/string.c
$ffly_cc -c $cc_flags -o $dst_dir/file.o $root_dir/file.c

# $dst_dir/task_worker.o $dst_dir/task.o $dst_dir/task_pool.o $dst_dir/time.o
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
$dst_dir/sys_nanosleep.o.0 $dst_dir/sys_nanosleep.o.1 \
$dst_dir/thread.o $dst_dir/config.o \
$dst_dir/cond_lock.o $dst_dir/err.o $dst_dir/realpath.o \
$dst_dir/sys_exit.o.0 $dst_dir/sys_exit.o.1 \
$dst_dir/sys_stat.o.0 $dst_dir/sys_stat.o.1 \
$dst_dir/sys_lseek.o.0 $dst_dir/sys_lseek.o.1 \
$dst_dir/sys_creat.o.0 $dst_dir/sys_creat.o.1 \
$dst_dir/sys_read.o.0 $dst_dir/sys_read.o.1 \
$dst_dir/sys_access.o.0 $dst_dir/sys_access.o.1 \
$dst_dir/sys_fsync.o.0 $dst_dir/sys_fsync.o.1 \
$dst_dir/sys_getpid.o.0 $dst_dir/sys_getpid.o.1 \
$dst_dir/sys_kill.o.0 $dst_dir/sys_kill.o.1 \
$dst_dir/sys_mmap.o.0 $dst_dir/sys_mmap.o.1 \
$dst_dir/sys_munmap.o.0 $dst_dir/sys_munmap.o.1 \
$dst_dir/sys_shmctl.o.0 $dst_dir/sys_shmctl.o.1
$dst_dir/sys_shmget.o.0 $dst_dir/sys_shmget.o.1
$dst_dir/sys_shmat.o.0 $dst_dir/sys_shmat.o.1 \
$dst_dir/sys_shmdt.o.0 $dst_dir/sys_shmdt.o.1 \
$dst_dir/sys_getcwd.o.0 $dst_dir/sys_getcwd.o.1
$dst_dir/sys_clone.o.0 $dst_dir/sys_clone.o.1"
