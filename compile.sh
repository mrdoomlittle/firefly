#!/bin/sh
. ./configure.sh "$3 $4"
echo "buildiing target: '$ffly_target'"
echo "src: $2, dest: $1"
echo "extra options: $4"

ffly_defines=
ffly_linker=
if [ $ffly_target = "ffly_client" ]; then
	ffly_linker=-lffly_client
elif [ $ffly_target = "ffly_server" ]; then
	ffly_linker=-lffly_server
elif [ $ffly_target = "ffly_studio" ]; then
	ffly_linker=-lffly_studio
elif [ $ffly_target = "ffly_worker" ]; then
	ffly_linker=-lffly_worker
elif [ $ffly_target = "ffly_test" ]; then
	ffly_linker=-lffly_test
elif [ $ffly_target = "ffly_bare"]; then
	ffly_linker=-lffly_bare
fi

if [ $ffly_cl = "cuda" ]; then
	ffly_defines="$ffly_defines -D__ffly_use_cuda"
elif [ $ffly_cl = "opencl" ];then
	ffly_defines="$ffly_defines -D__ffly_use_opencl"
fi

ffly_window=
if [ $ffly_use_x11 -eq 1 ]; then
	ffly_window=x11
	ffly_defines="$ffly_defines -D__ffly_use_x11"
elif [ $ffly_use_xcb -eq 1 ]; then
	ffly_window=xcb
	ffly_defines="$ffly_defines -D__ffly_use_xcb"
fi

if [ $ffly_debug_enabled -eq 1 ]; then
	ffly_defines="$ffly_defines -D__ffly_debug_enabled"
fi

# memory allocation tracker
if [ $ffly_mal_track -eq 1 ]; then
	ffly_defines="$ffly_defines -D__ffly_mal_track"
fi

if [ $ffly_use_allocr -eq 1 ]; then
	ffly_defines="$ffly_defines -D__ffly_use_allocr"
fi

. ./cac_dirs.bash
. ./copy_headers.bash
. ./mac_libs.bash

make -j2 ffly_target="$ffly_target" \
ffly_stdc="$ffly_stdc" \
ffly_stdcxx="$ffly_stdcxx" \
ffly_cc="$ffly_cc" \
ffly_cxx="$ffly_cxx" \
ffly_nvcc="$ffly_nvcc" \
ffly_inc_flags="$ffly_inc_flags" \
ffly_lib_flags="$ffly_lib_flags" \
ffly_ld_flags="$ffly_ld_flags" \
ffly_cc_inc_flags="$ffly_cc_inc_flags" \
ffly_cc_lib_flags="$ffly_cc_lib_flags" \
ffly_cc_ld_flags="$ffly_cc_ld_flags" \
ffly_cxx_inc_flags="$ffly_cxx_inc_flags" \
ffly_cxx_lib_flags="$ffly_cxx_lib_flags" \
ffly_cxx_ld_flags="$ffly_cxx_ld_flags" \
ffly_nasm_flags="$ffly_nasm_flags" \
ffly_cflags="$ffly_cflags" \
ffly_ccflags="$ffly_ccflags" \
ffly_cxxflags="$ffly_cxxflags" \
ffly_arc="$ffly_arc" \
ffly_cl="$ffly_cl" \
ffly_window="$ffly_window" \
ffly_objs="$ffly_objs" \
ffly_defines="$ffly_defines" \
ffly_flags="$ffly_flags"
#make -j4 FFLY_TARGET=$ffly_target FFLY_WINDOW=$ffly_window ffly_defines

echo "target: $ffly_target"
echo "defines: $ffly_defines"
echo "flags: $ffly_flags"
echo "dest: $1"
g++ -std=c++17 -D__$arc $ffly_cxxflags $ffly_cflags -D__$ffly_target $ffly_defines -Iinc -Llib -Wall -o $1 $2 /usr/lib/x86_64-linux-gnu/libGLU.so.1 $ffly_linker $ffly_ld_flags -nostartfiles -nostdlib
echo "finished."
