#!/bin/sh
as -o $dst_dir/round.o.0 $root_dir/asm/round.s
$ffly_cc -c -o $dst_dir/round.o.1 $root_dir/round.c

as -o $dst_dir/floor.o.0 $root_dir/asm/floor.s
$ffly_cc -c -o $dst_dir/floor.o.1 $root_dir/floor.c

as -o $dst_dir/ceil.o.0 $root_dir/asm/ceil.s
$ffly_cc -c -o $dst_dir/ceil.o.1 $root_dir/ceil.c

as -o $dst_dir/sqr.o.0 $root_dir/asm/sqr.s
$ffly_cc -c -o $dst_dir/sqr.o.1 $root_dir/sqr.c

as -o $dst_dir/cos.o.0 $root_dir/asm/cos.s
$ffly_cc -c -o $dst_dir/cos.o.1 $root_dir/cos.c

as -o $dst_dir/sin.o.0 $root_dir/asm/sin.s
$ffly_cc -c -o $dst_dir/sin.o.1 $root_dir/sin.c

$ffly_cc -c -o $dst_dir/vec2.o $root_dir/vec2.c
$ffly_cc -c -o $dst_dir/vec3.o $root_dir/vec3.c

$ffly_cc -c -o $dst_dir/min.o $root_dir/min.c
$ffly_cc -c -o $dst_dir/max.o $root_dir/max.c

$ffly_cc -c -o $dst_dir/barycentric.o $root_dir/barycentric.c
$ffly_cc -c -o $dst_dir/rotate.o $root_dir/rotate.c
export ffly_objs="$dst_dir/round.o.0 $dst_dir/round.o.1 \
$dst_dir/floor.o.0 $dst_dir/floor.o.1 \
$dst_dir/ceil.o.0 $dst_dir/ceil.o.1 \
$dst_dir/sqr.o.0 $dst_dir/sqr.o.1 \
$dst_dir/cos.o.0 $dst_dir/cos.o.1 \
$dst_dir/sin.o.0 $dst_dir/sin.o.1 \
$dst_dir/vec2.o $dst_dir/vec3.o \
$dst_dir/min.o $dst_dir/max.o \
$dst_dir/barycentric.o $dst_dir/rotate.o"
