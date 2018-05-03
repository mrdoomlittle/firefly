#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/mem_swp.o $root_dir/mem_swp.c
export ffly_objs="$dst_dir/mem_swp.o"
