#!/bin/sh
as -o $dst_dir/memmov128.o $root_dir/memmov128.s
as -o $dst_dir/memmov256.o $root_dir/memmov256.s
as -o $dst_dir/memmov512.o $root_dir/memmov512.s
export ffly_objs="$dst_dir/memmov128.o $dst_dir/memmov256.o $dst_dir/memmov512.o"
