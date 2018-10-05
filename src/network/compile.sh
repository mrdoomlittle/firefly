#!/bin/sh
$ffly_cc $cc_flags -c -o $dst_dir/input.o $root_dir/input.c
$ffly_cc $cc_flags -c -o $dst_dir/output.o $root_dir/output.c
$ffly_cc $cc_flags -c -o $dst_dir/sock.o $root_dir/sock.c
$ffly_cc $cc_flags -c -o $dst_dir/tcp.o $root_dir/tcp.c
$ffly_cc $cc_flags -c -o $dst_dir/udp.o $root_dir/udp.c
export ffly_objs="$dst_dir/input.o $dst_dir/output.o $dst_dir/sock.o $dst_dir/tcp.o $dst_dir/udp.o"
