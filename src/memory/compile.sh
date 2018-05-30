#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/mem_alloc.o.1 $root_dir/mem_alloc.c
cpp $cc_flags -o $root_dir/mem_alloc.s $root_dir/mem_alloc.S
as -o $dst_dir/mem_alloc.o.0 $root_dir/mem_alloc.s
$ffly_cc $cc_flags -c -o $dst_dir/mem_free.o $root_dir/mem_free.c
$ffly_cc $cc_flags -c -o $dst_dir/mem_realloc.o $root_dir/mem_realloc.c
$ffly_cc $cc_flags -c -o $dst_dir/allocr.o $root_dir/allocr.c
$ffly_cc $cc_flags -c -o $dst_dir/alloca.o $root_dir/alloca.c
$ffly_cc $cc_flags -c -o $dst_dir/plate.o $root_dir/plate.c
export ffly_objs="$dst_dir/mem_alloc.o.1 $dst_dir/mem_alloc.o.0 $dst_dir/mem_free.o $dst_dir/mem_realloc.o $dst_dir/allocr.o $dst_dir/alloca.o $dst_dir/plate.o"
