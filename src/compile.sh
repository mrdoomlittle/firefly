#!/bin/sh
ffly_cc=gcc
cc_flags="-fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr"
ffly_objs=$(
	dst_dir=$PWD/dep
	root_dir=dep;
	. ./dep/compile.sh;
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
$ffly_cc $cc_flags -c -o $dst_dir/mode.o $root_dir/mode.c
$ffly_cc $cc_flags -c -o $dst_dir/config.o $root_dir/config.c
$ffly_cc $cc_flags -c -o $dst_dir/ffly_system.o $root_dir/ffly_system.c
$ffly_cc $cc_flags -c -o $dst_dir/firefly.o $root_dir/firefly.c
$ffly_cc $cc_flags -c -o $dst_dir/put_bit.o $root_dir/put_bit.c
$ffly_cc $cc_flags -c -o $dst_dir/get_bit.o $root_dir/get_bit.c

$ffly_cc $cc_flags -c -o $dst_dir/arcs.o $root_dir/arcs.c
export ffly_objs="$ffly_objs $dst_dir/mode.o $dst_dir/firefly.o $dst_dir/ffly_system.o $dst_dir/config.o $dst_dir/put_bit.o  $dst_dir/get_bit.o $dst_dir/arcs.o"
