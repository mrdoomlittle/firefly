#!/bin/sh
nasm -f elf64 -o $dst_dir/round.o.0 $root_dir/asm/round.asm
$ffly_cc -c -o $dst_dir/round.o.1 $root_dir/round.c
export ffly_objs="$dst_dir/round.o.0 $dst_dir/round.o.1"
