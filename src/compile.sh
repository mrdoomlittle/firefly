#!/bin/sh
ffly_cc=gcc
cc_flags=-D__ffly_no_task_pool
ffly_objs=$(
	dst_dir=$PWD/data
	root_dir=data;
	. ./data/compile.sh;
	echo "$ffly_objs"
)

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/system
	root_dir=system;
	. ./system/compile.sh;
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/maths
	root_dir=maths;
	. ./maths/compile.sh;
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/memory
	root_dir=memory;
	. ./memory/compile.sh;
	echo "$ffly_objs"
)"

dst_dir=$PWD
root_dir=.
$ffly_cc $cc_flags -c -o $dst_dir/config.o $root_dir/config.c
$ffly_cc $cc_flags -c -o $dst_dir/ffly_system.o $root_dir/ffly_system.c
$ffly_cc $cc_flags -c -o $dst_dir/firefly.o $root_dir/firefly.c
export ffly_objs="$ffly_objs $dst_dir/firefly.o $dst_dir/ffly_system.o $dst_dir/config.o"
