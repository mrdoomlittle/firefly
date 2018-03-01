#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/bcopy.o.1 $root_dir/bcopy.c
as -o $dst_dir/bcopy.o.0 $root_dir/bcopy.s
$ffly_cc $cc_flags -c -o $dst_dir/bzero.o.1 $root_dir/bzero.c
as -o $dst_dir/bzero.o.0 $root_dir/bzero.s
$ffly_cc $cc_flags -c -o $dst_dir/mem_cpy.o.1 $root_dir/mem_cpy.c
as -o $dst_dir/mem_cpy.o.0 $root_dir/mem_cpy.s
$ffly_cc $cc_flags -c -o $dst_dir/mem_cmp.o.1 $root_dir/mem_cmp.c
as -o $dst_dir/mem_cmp.o.0 $root_dir/mem_cmp.s
$ffly_cc $cc_flags -c -o $dst_dir/str_len.o.1 $root_dir/str_len.c
as -o $dst_dir/str_len.o.0 $root_dir/str_len.s
$ffly_cc $cc_flags -c -o $dst_dir/str_cmp.o.1 $root_dir/str_cmp.c
as -o $dst_dir/str_cmp.o.0 $root_dir/str_cmp.s
$ffly_cc $cc_flags -c -o $dst_dir/str_cpy.o.1 $root_dir/str_cpy.c
as -o $dst_dir/str_cpy.o.0 $root_dir/str_cpy.s
$ffly_cc $cc_flags -c -o $dst_dir/mem_dup.o.1 $root_dir/mem_dup.c
as -o $dst_dir/mem_dup.o.0 $root_dir/mem_dup.s
$ffly_cc $cc_flags -c -o $dst_dir/str_dup.o.1 $root_dir/str_dup.c
as -o $dst_dir/str_dup.o.0 $root_dir/str_dup.s
$ffly_cc $cc_flags -c -o $dst_dir/mem_set.o.1 $root_dir/mem_set.c
as -o $dst_dir/mem_set.o.0 $root_dir/mem_set.s
export ffly_objs="$dst_dir/bcopy.o.1 $dst_dir/bcopy.o.0 \
$dst_dir/bzero.o.1 $dst_dir/bzero.o.0 \
$dst_dir/mem_cpy.o.1 $dst_dir/mem_cpy.o.0 \
$dst_dir/mem_cmp.o.1 $dst_dir/mem_cmp.o.0 \
$dst_dir/str_len.o.1 $dst_dir/str_len.o.0 \
$dst_dir/str_cmp.o.1 $dst_dir/str_cmp.o.0 \
$dst_dir/str_cpy.o.1 $dst_dir/str_cpy.o.0 \
$dst_dir/mem_dup.o.1 $dst_dir/mem_dup.o.0 \
$dst_dir/str_dup.o.1 $dst_dir/str_dup.o.0 \
$dst_dir/mem_set.o.1 $dst_dir/mem_set.o.0"
