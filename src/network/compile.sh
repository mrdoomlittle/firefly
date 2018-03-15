#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/input.o $root_dir/input.c
$ffly_cc $cc_flags -c -o $dst_dir/output.o $root_dir/output.c
$ffly_cc $cc_flags -c -o $dst_dir/sock.o $root_dir/sock.c
export ffly_objs="$dst_dir/input.o $dst_dir/output.o $dst_dir/sock.o"
