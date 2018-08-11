#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/mem_swp.o $root_dir/mem_swp.c
$ffly_cc $cc_flags -c -o $dst_dir/str_cmb.o $root_dir/str_cmb.c
$ffly_cc $cc_flags -c -o $dst_dir/str_put.o $root_dir/str_put.c
export ffly_objs="$dst_dir/mem_swp.o $dst_dir/str_cmb.o $dst_dir/str_put.o"
