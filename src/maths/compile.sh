#!/bin/sh
as -o $dst_dir/round.o.0 $root_dir/asm/round.s
$ffly_cc -c -o $dst_dir/round.o.1 $root_dir/round.c

as -o $dst_dir/floor.o.0 $root_dir/asm/floor.s
$ffly_cc -c -o $dst_dir/floor.o.1 $root_dir/floor.c

as -o $dst_dir/ceil.o.0 $root_dir/asm/ceil.s
$ffly_cc -c -o $dst_dir/ceil.o.1 $root_dir/ceil.c

as -o $dst_dir/sqr.o.0 $root_dir/asm/sqr.s
$ffly_cc -c -o $dst_dir/sqr.o.1 $root_dir/sqr.c

#as -o $dst_dir/cos.o.0 $root_dir/asm/cos.s
#$ffly_cc -c -o $dst_dir/cos.o.1 $root_dir/cos.c

#as -o $dst_dir/sin.o.0 $root_dir/asm/sin.s
#$ffly_cc -c -o $dst_dir/sin.o.1 $root_dir/sin.c
export ffly_objs="$dst_dir/round.o.0 $dst_dir/round.o.1 \
$dst_dir/floor.o.0 $dst_dir/floor.o.1 \
$dst_dir/ceil.o.0 $dst_dir/ceil.o.1 \
$dst_dir/sqr.o.0 $dst_dir/sqr.o.1"
#$dst_dir/cos.o.0 $dst_dir/cos.o.1 \
#$dst_dir/sin.o.0 $dst_dir/sin.o.1"
