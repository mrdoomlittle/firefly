#!/bin/sh
. ./configure.sh "$3 $4"
echo "buildiing target: '$ffly_target'"
echo "src: $2, dest: $1"
echo "extra options: $4"

ffly_defines=
ffly_linker=
if [ $ffly_target = "FFLY_CLIENT" ]; then
	ffly_linker=-lffly_client
elif [ $ffly_target = "FFLY_SERVER" ]; then
	ffly_linker=-lffly_server
elif [ $ffly_target = "FFLY_STUDIO" ]; then
	ffly_linker=-lffly_studio
elif [ $ffly_target = "FFLY_WORKER" ]; then
	ffly_linker=-lffly_worker
elif [ $ffly_target = "FFLY_TEST" ]; then
	ffly_linker=-lffly_test
fi

if [ $ffly_cl = "cuda" ]; then
	ffly_defines="$ffly_defined -D__USING_CUDA"
fi

ffly_window=
if [ $ffly_using_x11 -eq 1 ]; then
	ffly_window=X11
	ffly_defines="$ffly_defines -D__USING_X11"
elif [ $ffly_using_xcb -eq 1 ]; then
	ffly_window=XCB
	ffly_defines="$ffly_defines -D__USING_XCB"
fi

if [ $ffly_with_room_manager -eq 1 ]; then
	ffly_defines="$ffly_defines -D__WITH_ROOM_MANAGER"
fi

if [ $ffly_with_obj_manager -eq 1 ]; then
	ffly_defines="$ffly_defines -D__WITH_OBJ_MANAGER"
fi

if [ $ffly_rm_layering -eq 1 ]; then
	ffly_defines="$ffly_defines -D__RM_LAYERING"
fi

if [ $ffly_with_uni_manager -eq 1 ]; then
	ffly_defines="$ffly_defines -D__WITH_UNI_MANAGER"
fi

if [ $ffly_debug_enabled -eq 1 ]; then
	ffly_defines="$ffly_defines -D__DEBUG_ENABLED"
fi

if [ $ffly_mem_agent -eq 1 ]; then
	ffly_defines="$ffly_defines -D__MEM_AGENT"
fi

if [ $ffly_with_layer_manager -eq 1 ]; then
	ffly_defines="$ffly_defines -D__WITH_LAYER_MANAGER"
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
g++ -std=c++17 -D__$arc $ffly_cxxflags $ffly_cflags -D$ffly_target $ffly_defines -Iinc -Llib -Wall -o $1 $2 /usr/lib/x86_64-linux-gnu/libGLU.so.1 $ffly_linker $ffly_ld_flags
echo "finished."
