#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/bcopy.o $root_dir/bcopy.c
$ffly_cc $cc_flags -c -o $dst_dir/bzero.o $root_dir/bzero.c
$ffly_cc $cc_flags -c -o $dst_dir/str_cmp.o $root_dir/str_cmp.c
$ffly_cc $cc_flags -c -o $dst_dir/str_cpy.o $root_dir/str_cpy.c
$ffly_cc $cc_flags -c -o $dst_dir/str_dupe.o $root_dir/str_dupe.c
$ffly_cc $cc_flags -c -o $dst_dir/str_len.o $root_dir/str_len.c
$ffly_cc $cc_flags -c -o $dst_dir/mem_dupe.o $root_dir/mem_dupe.c
$ffly_cc $cc_flags -c -o $dst_dir/mem_cmp.o $root_dir/mem_cmp.c
$ffly_cc $cc_flags -c -o $dst_dir/mem_set.o $root_dir/mem_set.c
$ffly_cc $cc_flags -c -o $dst_dir/mem_cpy.o $root_dir/mem_cpy.c
export ffly_objs="$dst_dir/bcopy.o $dst_dir/bzero.o $dst_dir/str_cmp.o \
$dst_dir/str_cpy.o $dst_dir/str_dupe.o $dst_dir/str_len.o \
$dst_dir/mem_dupe.o $dst_dir/mem_cmp.o $dst_dir/mem_set.o $dst_dir/mem_cpy.o"
