#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/light.o $root_dir/light.c
$ffly_cc $cc_flags -c -o $dst_dir/body.o $root_dir/body.c
$ffly_cc $cc_flags -c -o $dst_dir/clock.o $root_dir/clock.c
$ffly_cc $cc_flags -c -o $dst_dir/contact.o $root_dir/contact.c
export ffly_objs="$dst_dir/light.o $dst_dir/body.o $dst_dir/clock.o $dst_dir/contact.o"
