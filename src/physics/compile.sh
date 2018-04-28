#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/light.o $root_dir/light.c
export ffly_objs="$dst_dir/light.o"
