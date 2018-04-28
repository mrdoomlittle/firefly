#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/frame_buff.o $root_dir/frame_buff.c
export ffly_objs="$dst_dir/frame_buff.o"
