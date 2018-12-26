#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/frame_buff.o $root_dir/frame_buff.c
$ffly_cc $cc_flags -c -o $dst_dir/job.o $root_dir/job.c
$ffly_cc $cc_flags -c -o $dst_dir/pipe.o $root_dir/pipe.c
$ffly_cc $cc_flags -c -o $dst_dir/draw.o $root_dir/draw.c
$ffly_cc $cc_flags -c -o $dst_dir/copy.o $root_dir/copy.c
$ffly_cc $cc_flags -c -o $dst_dir/fill.o $root_dir/fill.c
$ffly_cc $cc_flags -c -o $dst_dir/p.o $root_dir/p.c
$ffly_cc $cc_flags -c -o $dst_dir/image.o $root_dir/image.c
$ffly_cc $cc_flags -c -o $dst_dir/mutate.o $root_dir/mutate.c
$ffly_cc $cc_flags -c -o $dst_dir/mare.o $root_dir/mare.c
$ffly_cc $cc_flags -c -o $dst_dir/chamber.o $root_dir/chamber.c
$ffly_cc $cc_flags -c -o $dst_dir/dk.o $root_dir/dk.c
export ffly_objs="$dst_dir/frame_buff.o $dst_dir/job.o $dst_dir/pipe.o \
$dst_dir/draw.o $dst_dir/copy.o $dst_dir/fill.o $dst_dir/p.o $dst_dir/image.o $dst_dir/mutate.o $dst_dir/mare.o $dst_dir/chamber.o $dst_dir/dk.o"
