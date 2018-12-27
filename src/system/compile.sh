#!/bin/sh
as_inc="-I$root_dir/asm/ -I$(realpath $root_dir/../)/linux/asm/"
$ffly_cc -c $cc_flags -o $dst_dir/realpath.o $root_dir/realpath.c
$ffly_cc -c $cc_flags -o $dst_dir/err.o $root_dir/err.c

nasm -f elf64 $as_inc -o $dst_dir/cond_lock.o.0 $root_dir/asm/cond_lock.asm
$ffly_cc -c $cc_flags -o $dst_dir/cond_lock.o.1 $root_dir/cond_lock.c

$ffly_cc -c $cc_flags -o $dst_dir/config.o $root_dir/config.c
$ffly_cc -c $cc_flags -o $dst_dir/thread.o $root_dir/thread.c
$ffly_cc -c $cc_flags -o $dst_dir/util/base64.o $root_dir/util/base64.c
#$ffly_cc -c $cc_flags -o $dst_dir/time.o $root_dir/time.c
#$ffly_cc -c $cc_flags -o $dst_dir/task_worker.o $root_dir/task_worker.c
#$ffly_cc -c $cc_flags -o $dst_dir/task.o $root_dir/task.c
#$ffly_cc -c $cc_flags -o $dst_dir/task_pool.o $root_dir/task_pool.c

$ffly_cc -c $cc_flags -o $dst_dir/bin_tree.o $root_dir/bin_tree.c
$ffly_cc -c $cc_flags -o $dst_dir/buff.o $root_dir/buff.c
$ffly_cc -c $cc_flags -o $dst_dir/vec.o $root_dir/vec.c
$ffly_cc -c $cc_flags -o $dst_dir/map.o $root_dir/map.c
$ffly_cc -c $cc_flags -o $dst_dir/hash.o $root_dir/util/hash.c
$ffly_cc -c $cc_flags -o $dst_dir/lat.o $root_dir/lat.c
$ffly_cc -c $cc_flags -o $dst_dir/dict.o $root_dir/dict.c
$ffly_cc -c $cc_flags -o $dst_dir/barrel.o $root_dir/barrel.c

nasm -f elf64 $as_inc -o $dst_dir/atomic_op.o.0 $root_dir/asm/atomic_op.asm
$ffly_cc -c $cc_flags -o $dst_dir/atomic_op.o.1 $root_dir/atomic_op.c

$ffly_cc -c $cc_flags -o $dst_dir/pipe.o $root_dir/pipe.c
$ffly_cc -c $cc_flags -o $dst_dir/shm.o $root_dir/shm.c
$ffly_cc -c $cc_flags -o $dst_dir/errno.o $root_dir/errno.c
$ffly_cc -c $cc_flags -o $dst_dir/io.o $root_dir/io.c

nasm -f elf64 $as_inc -o $dst_dir/sys_timer_settime.o.0 $root_dir/asm/sys_timer_settime.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_timer_settime.o.1 $root_dir/sys_timer_settime.c

nasm -f elf64 $as_inc -o $dst_dir/sys_timer_create.o.0 $root_dir/asm/sys_timer_create.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_timer_create.o.1 $root_dir/sys_timer_create.c

nasm -f elf64 $as_inc -o $dst_dir/sys_timer_delete.o.0 $root_dir/asm/sys_timer_delete.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_timer_delete.o.1 $root_dir/sys_timer_delete.c

nasm -f elf64 $as_inc -o $dst_dir/sys_mkdir.o.0 $root_dir/asm/sys_mkdir.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_mkdir.o.1 $root_dir/sys_mkdir.c

nasm -f elf64 $as_inc -o $dst_dir/sys_dup2.o.0 $root_dir/asm/sys_dup2.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_dup2.o.1 $root_dir/sys_dup2.c

nasm -f elf64 $as_inc -o $dst_dir/sys_arch_prctl.o.0 $root_dir/asm/sys_arch_prctl.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_arch_prctl.o.1 $root_dir/sys_arch_prctl.c

nasm -f elf64 $as_inc -o $dst_dir/sys_rt_sigaction.o.0 $root_dir/asm/sys_rt_sigaction.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_rt_sigaction.o.1 $root_dir/sys_rt_sigaction.c

nasm -f elf64 $as_inc -o $dst_dir/sys_nanosleep.o.0 $root_dir/asm/sys_nanosleep.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_nanosleep.o.1 $root_dir/sys_nanosleep.c

nasm -f elf64 $as_inc -o $dst_dir/sys_clock_gettime.o.0 $root_dir/asm/sys_clock_gettime.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_clock_gettime.o.1 $root_dir/sys_clock_gettime.c

nasm -f elf64 $as_inc -o $dst_dir/mutex.o.0 $root_dir/asm/mutex.asm
$ffly_cc -c $cc_flags -o $dst_dir/mutex.o.1 $root_dir/mutex.c

nasm -f elf64 $as_inc -o $dst_dir/sys_ioctl.o.0 $root_dir/asm/sys_ioctl.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_ioctl.o.1 $root_dir/sys_ioctl.c

nasm -f elf64 $as_inc -o $dst_dir/sys_unlink.o.0 $root_dir/asm/sys_unlink.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_unlink.o.1 $root_dir/sys_unlink.c

nasm -f elf64 $as_inc -o $dst_dir/sys_shutdown.o.0 $root_dir/asm/sys_shutdown.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_shutdown.o.1 $root_dir/sys_shutdown.c

nasm -f elf64 $as_inc -o $dst_dir/sys_connect.o.0 $root_dir/asm/sys_connect.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_connect.o.1 $root_dir/sys_connect.c

nasm -f elf64 $as_inc -o $dst_dir/sys_bind.o.0 $root_dir/asm/sys_bind.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_bind.o.1 $root_dir/sys_bind.c

nasm -f elf64 $as_inc -o $dst_dir/sys_accept.o.0 $root_dir/asm/sys_accept.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_accept.o.1 $root_dir/sys_accept.c

nasm -f elf64 $as_inc -o $dst_dir/sys_listen.o.0 $root_dir/asm/sys_listen.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_listen.o.1 $root_dir/sys_listen.c

nasm -f elf64 $as_inc -o $dst_dir/sys_socket.o.0 $root_dir/asm/sys_socket.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_socket.o.1 $root_dir/sys_socket.c

nasm -f elf64 $as_inc -o $dst_dir/sys_sendto.o.0 $root_dir/asm/sys_sendto.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_sendto.o.1 $root_dir/sys_sendto.c

nasm -f elf64 $as_inc -o $dst_dir/sys_recvfrom.o.0 $root_dir/asm/sys_recvfrom.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_recvfrom.o.1 $root_dir/sys_recvfrom.c

nasm -f elf64 $as_inc -o $dst_dir/sys_setsockopt.o.0 $root_dir/asm/sys_setsockopt.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_setsockopt.o.1 $root_dir/sys_setsockopt.c

nasm -f elf64 $as_inc -o $dst_dir/sys_getsockopt.o.0 $root_dir/asm/sys_getsockopt.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_getsockopt.o.1 $root_dir/sys_getsockopt.c

nasm -f elf64 $as_inc -o $dst_dir/sys_ftruncate.o.0 $root_dir/asm/sys_ftruncate.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_ftruncate.o.1 $root_dir/sys_ftruncate.c

nasm -f elf64 $as_inc -o $dst_dir/sys_execve.o.0 $root_dir/asm/sys_execve.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_execve.o.1 $root_dir/sys_execve.c

nasm -f elf64 $as_inc -o $dst_dir/sys_rmdir.o.0 $root_dir/asm/sys_rmdir.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_rmdir.o.1 $root_dir/sys_rmdir.c

nasm -f elf64 $as_inc -o $dst_dir/sys_fork.o.0 $root_dir/asm/sys_fork.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_fork.o.1 $root_dir/sys_fork.c

nasm -f elf64 $as_inc -o $dst_dir/sys_fcntl.o.0 $root_dir/asm/sys_fcntl.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_fcntl.o.1 $root_dir/sys_fcntl.c

nasm -f elf64 $as_inc -o $dst_dir/sys_wait4.o.0 $root_dir/asm/sys_wait4.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_wait4.o.1 $root_dir/sys_wait4.c

nasm -f elf64 $as_inc -o $dst_dir/sys_open.o.0 $root_dir/asm/sys_open.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_open.o.1 $root_dir/sys_open.c

nasm -f elf64 $as_inc -o $dst_dir/sys_close.o.0 $root_dir/asm/sys_close.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_close.o.1 $root_dir/sys_close.c

nasm -f elf64 $as_inc -o $dst_dir/sys_write.o.0 $root_dir/asm/sys_write.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_write.o.1 $root_dir/sys_write.c

nasm -f elf64 $as_inc -o $dst_dir/sys_brk.o.0 $root_dir/asm/sys_brk.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_brk.o.1 $root_dir/sys_brk.c

nasm -f elf64 $as_inc -o $dst_dir/sys_exit.o.0 $root_dir/asm/sys_exit.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_exit.o.1 $root_dir/sys_exit.c

nasm -f elf64 $as_inc -o $dst_dir/sys_stat.o.0 $root_dir/asm/sys_stat.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_stat.o.1 $root_dir/sys_stat.c

nasm -f elf64 $as_inc -o $dst_dir/sys_lseek.o.0 $root_dir/asm/sys_lseek.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_lseek.o.1 $root_dir/sys_lseek.c

nasm -f elf64 $as_inc -o $dst_dir/sys_creat.o.0 $root_dir/asm/sys_creat.asm
$ffly_cc -c $cc_flags -o $dst_dir/sys_creat.o.1 $root_dir/sys_creat.c

nasm -f elf64 $as_inc -o $dst_dir/sys_read.o.0 $root_dir/asm/sys_read.asm
$ffly_cc -c $cc_flags -c -o $dst_dir/sys_read.o.1 $root_dir/sys_read.c

nasm -f elf64 $as_inc -o $dst_dir/sys_access.o.0 $root_dir/asm/sys_access.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_access.o.1 $root_dir/sys_access.c

nasm -f elf64 $as_inc -o $dst_dir/sys_fsync.o.0 $root_dir/asm/sys_fsync.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_fsync.o.1 $root_dir/sys_fsync.c

nasm -f elf64 $as_inc -o $dst_dir/sys_getpid.o.0 $root_dir/asm/sys_getpid.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_getpid.o.1 $root_dir/sys_getpid.c

nasm -f elf64 $as_inc -o $dst_dir/sys_kill.o.0 $root_dir/asm/sys_kill.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_kill.o.1 $root_dir/sys_kill.c

nasm -f elf64 $as_inc -o $dst_dir/sys_mmap.o.0 $root_dir/asm/sys_mmap.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_mmap.o.1 $root_dir/sys_mmap.c

nasm -f elf64 $as_inc -o $dst_dir/sys_munmap.o.0 $root_dir/asm/sys_munmap.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_munmap.o.1 $root_dir/sys_munmap.c

nasm -f elf64 $as_inc -o $dst_dir/sys_shmctl.o.0 $root_dir/asm/sys_shmctl.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmctl.o.1 $root_dir/sys_shmctl.c

nasm -f elf64 $as_inc -o $dst_dir/sys_shmget.o.0 $root_dir/asm/sys_shmget.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmget.o.1 $root_dir/sys_shmget.c

nasm -f elf64 $as_inc -o $dst_dir/sys_shmat.o.0 $root_dir/asm/sys_shmat.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmat.o.1 $root_dir/sys_shmat.c

nasm -f elf64 $as_inc -o $dst_dir/sys_shmdt.o.0 $root_dir/asm/sys_shmdt.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_shmdt.o.1 $root_dir/sys_shmdt.c

nasm -f elf64 $as_inc -o $dst_dir/sys_getcwd.o.0 $root_dir/asm/sys_getcwd.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_getcwd.o.1 $root_dir/sys_getcwd.c

nasm -f elf64 $as_inc -o $dst_dir/sys_clone.o.0 $root_dir/asm/sys_clone.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_clone.o.1 $root_dir/sys_clone.c

nasm -f elf64 $as_inc -o $dst_dir/sys_fstat.o.0 $root_dir/asm/sys_fstat.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_fstat.o.1 $root_dir/sys_fstat.c

nasm -f elf64 $as_inc -o $dst_dir/sys_pwrite.o.0 $root_dir/asm/sys_pwrite.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_pwrite.o.1 $root_dir/sys_pwrite.c

nasm -f elf64 $as_inc -o $dst_dir/sys_pread.o.0 $root_dir/asm/sys_pread.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_pread.o.1 $root_dir/sys_pread.c

nasm -f elf64 $as_inc -o $dst_dir/sys_writev.o.0 $root_dir/asm/sys_writev.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_writev.o.1 $root_dir/sys_writev.c

nasm -f elf64 $as_inc -o $dst_dir/sys_readv.o.0 $root_dir/asm/sys_readv.asm
$ffly_cc $cc_flags -c -o $dst_dir/sys_readv.o.1 $root_dir/sys_readv.c

$ffly_cc -c $cc_flags -o $dst_dir/sched.o $root_dir/sched.c

$ffly_cc -c $cc_flags -o $dst_dir/send.o $root_dir/send.c
$ffly_cc -c $cc_flags -o $dst_dir/recv.o $root_dir/recv.c

$ffly_cc -c $cc_flags -D__ffly_debug -o $dst_dir/printf.o $root_dir/printf.c
$ffly_cc -c $cc_flags -o $dst_dir/string.o $root_dir/string.c
$ffly_cc -c $cc_flags -o $dst_dir/file.o $root_dir/file.c

$ffly_cc -c $cc_flags -o $dst_dir/servant.o $root_dir/servant.c
$ffly_cc -c $cc_flags -o $dst_dir/task_pool.o $root_dir/task_pool.c

$ffly_cc -c $cc_flags -o $dst_dir/event.o $root_dir/event.c
$ffly_cc -c $cc_flags -o $dst_dir/queue.o $root_dir/queue.c
$ffly_cc -c $cc_flags -o $dst_dir/port.o $root_dir/port.c
$ffly_cc -c $cc_flags -o $dst_dir/tls.o $root_dir/tls.c
$ffly_cc -c $cc_flags -o $dst_dir/util/ff6.o $root_dir/util/ff6.c
$ffly_cc -c $cc_flags -o $dst_dir/util/ff5.o $root_dir/util/ff5.c
$ffly_cc -c $cc_flags -o $dst_dir/tcsetattr.o $root_dir/tcsetattr.c
$ffly_cc -c $cc_flags -o $dst_dir/tcgetattr.o $root_dir/tcgetattr.c
$ffly_cc -c $cc_flags -o $dst_dir/fs.o $root_dir/fs.c
$ffly_cc -c $cc_flags -o $dst_dir/posix_fs.o $root_dir/posix_fs.c

#$ffly_cc -c $cc_flags -o $dst_dir/mal_track.o $root_dir/mal_track.c

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
$dst_dir/vec.o $dst_dir/map.o $dst_dir/hash.o \
$dst_dir/buff.o $dst_dir/bin_tree.o \
$dst_dir/sys_nanosleep.o.0 $dst_dir/sys_nanosleep.o.1 \
$dst_dir/thread.o $dst_dir/config.o \
$dst_dir/cond_lock.o.0 $dst_dir/cond_lock.o.1 \
$dst_dir/err.o $dst_dir/realpath.o \
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
$dst_dir/sys_clone.o.0 $dst_dir/sys_clone.o.1 \
$dst_dir/sys_fstat.o.0 $dst_dir/sys_fstat.o.1 \
$dst_dir/lat.o $dst_dir/dict.o $dst_dir/util/base64.o \
$dst_dir/sys_execve.o.0 $dst_dir/sys_execve.o.1 \
$dst_dir/sys_wait4.o.0 $dst_dir/sys_wait4.o.1 \
$dst_dir/sys_fork.o.0 $dst_dir/sys_fork.o.1 \
$dst_dir/sys_rmdir.o.0 $dst_dir/sys_rmdir.o.1 \
$dst_dir/sys_mkdir.o.0 $dst_dir/sys_mkdir.o.1 \
$dst_dir/sys_unlink.o.0 $dst_dir/sys_unlink.o.1 \
$dst_dir/sys_shutdown.o.0 $dst_dir/sys_shutdown.o.1 \
$dst_dir/sys_connect.o.0 $dst_dir/sys_connect.o.1 \
$dst_dir/sys_bind.o.0 $dst_dir/sys_bind.o.1 \
$dst_dir/sys_accept.o.0 $dst_dir/sys_accept.o.1 \
$dst_dir/sys_listen.o.0 $dst_dir/sys_listen.o.1 \
$dst_dir/sys_socket.o.0 $dst_dir/sys_socket.o.1 \
$dst_dir/sys_sendto.o.0 $dst_dir/sys_sendto.o.1 \
$dst_dir/sys_recvfrom.o.0 $dst_dir/sys_recvfrom.o.1 \
$dst_dir/sys_pwrite.o.0 $dst_dir/sys_pwrite.o.1 \
$dst_dir/sys_pread.o.0 $dst_dir/sys_pread.o.1 \
$dst_dir/send.o $dst_dir/recv.o \
$dst_dir/sys_setsockopt.o.0 $dst_dir/sys_setsockopt.o.1 \
$dst_dir/sys_getsockopt.o.0 $dst_dir/sys_getsockopt.o.1 \
$dst_dir/sys_ftruncate.o.0  $dst_dir/sys_ftruncate.o.1 \
$dst_dir/sys_clock_gettime.o.0 $dst_dir/sys_clock_gettime.o.1 \
$dst_dir/sys_rt_sigaction.o.0 $dst_dir/sys_rt_sigaction.o.1 \
$dst_dir/barrel.o $dst_dir/sched.o $dst_dir/task_pool.o $dst_dir/servant.o \
$dst_dir/event.o $dst_dir/queue.o $dst_dir/port.o $dst_dir/tls.o
$dst_dir/sys_arch_prctl.o.0 $dst_dir/sys_arch_prctl.o.1 \
$dst_dir/util/ff6.o $dst_dir/sys_dup2.o.0 $dst_dir/sys_dup2.o.1 \
$dst_dir/sys_ioctl.o.0 $dst_dir/sys_ioctl.o.1 $dst_dir/tcsetattr.o $dst_dir/tcgetattr.o \
$dst_dir/sys_timer_settime.o.0 $dst_dir/sys_timer_settime.o.1 \
$dst_dir/sys_timer_create.o.0 $dst_dir/sys_timer_create.o.1 \
$dst_dir/sys_timer_delete.o.0 $dst_dir/sys_timer_delete.o.1 \
$dst_dir/util/ff5.o $dst_dir/fs.o $dst_dir/posix_fs.o \
$dst_dir/sys_writev.o.0 $dst_dir/sys_writev.o.1 $dst_dir/sys_readv.o.0 $dst_dir/sys_readv.o.1"
# $dst_dir/mal_track.o"
