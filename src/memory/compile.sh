#!/bin/sh
$ffly_cc -c -D__ffly_debug_enabled -o $dst_dir/mem_alloc.o $root_dir/mem_alloc.c
$ffly_cc -c -D__ffly_debug_enabled -o $dst_dir/mem_free.o $root_dir/mem_free.c
$ffly_cc -c -D__ffly_debug_enabled -o $dst_dir/mem_realloc.o $root_dir/mem_realloc.c
$ffly_cc -c -o $dst_dir/allocr.o $root_dir/allocr.c
export ffly_objs="$dst_dir/mem_alloc.o $dst_dir/mem_free.o $dst_dir/mem_realloc.o $dst_dir/allocr.o"
