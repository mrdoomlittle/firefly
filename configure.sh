#!/bin/sh
ffly_stdc="c11"
ffly_stdcxx="c++11"

ffly_flags=
ffly_objs=
ffly_cc="gcc"
ffly_cxx="g++"
ffly_nvcc="/usr/local/cuda-9.0/bin/nvcc"
ffly_cuda_dir="/usr/local/cuda-9.0"
ffly_cuda_inc_dir_name="include"
ffly_cuda_lib_dir_name="lib64"

ffly_inc_flags="-Inibbles/inc -Itermio/inc -Imdlint/inc -Iintlen/inc -Igetdigit/inc -Ito_string/inc -Istr_cmb/inc -Iserializer/inc -Itagged_memory/inc -Iemu2d/inc -Iemu3d/inc -Iechar_t/inc -I$ffly_cuda_dir/$ffly_cuda_inc_dir_name"
ffly_lib_flags="-Lnibbles/lib -Ltermio/lib -Lintlen/lib -Lgetdigit/lib -Lto_string/lib -Lstr_cmb/lib -Ltagged_memory/lib -Lemu2d/lib -Lemu3d/lib"
ffly_ld_flags="-lmdl-intlen -lmdl-getdigit -lmdl-to_string -lmdl-str_cmb -lmdl-emu2d -lmdl-emu3d -lpthread"

ffly_cc_inc_flags=
ffly_cc_lib_flags=
ffly_cc_ld_flags=

ffly_cc_inc_flags=
ffly_cc_lib_flags=
ffly_cc_ld_flags=
# compiler flags - global
ffly_cflags=-Wall

ffly_ccflags=$ffly_cflags
ffly_cxxflags=$ffly_cflags
ffly_arc=ARC$(getconf LONG_BIT)
ffly_cl=
ffly_force_cuda_use=0

ffly_root_dir=$(bash find.bash "$1" "--root-dir*")
if [ "$ffly_root_dir" = "1" ]; then
    ffly_root_dir=${PWD}
fi

if [ $(bash find.bash "$1" "--force-cuda") -eq 0 ]; then
	ffly_force_cuda_use=1
fi

if [ $(bash find.bash "$1" "--arc64") -eq 0 ]; then
	ffly_arc="ARC64"
fi

# not going to be used
if [ $(bash find.bash "$1" "--arc32") -eq 0 ]; then
	ffly_arc="ARC32"
fi

ffly_target=
if [ $(bash find.bash "$1" "--ffly-client") -eq 0 ]; then
	ffly_ld_flags="$ffly_ld_flags -lX11 -lGL -lglut -lX11-xcb -lxcb -lxcb-icccm -lpulse -lpulse-simple -lasound"
	ffly_target="ffly_client"
elif [ $(bash find.bash "$1" "--ffly-server") -eq 0 ]; then
	ffly_target="ffly_server"
elif [ $(bash find.bash "$1" "--ffly-studio") -eq 0 ]; then
	ffly_ld_flags="$ffly_ld_flags -lX11 -lGL -lGLU -lglut -lfreetype -lm -lpulse -lpulse-simple"
	ffly_inc_flags="$ffly_inc_flags -I/usr/include/freetype2"
	ffly_target="ffly_studio"
elif [ $(bash find.bash "$1" "--ffly-worker") -eq 0 ]; then
	ffly_target="ffly_worker"
elif [ $(bash find.bash "$1" "--ffly-test") -eq 0 ]; then
	ffly_ld_flags="$ffly_ld_flags -lX11 -lGL -lglut -lX11-xcb -lxcb -lxcb-icccm -lpulse -lpulse-simple -lasound"
	ffly_target="ffly_test"
elif [ $(bash find.bash "$1" "--ffly-bare") -eq 0 ]; then
#	ffly_ld_flags="$ffly_ld_flags"
	ffly_target="ffly_bare"
else
	echo "please specify target, --ffly-client; --ffly-studio; --ffly-worker; --ffly-test"
	return
fi

ffly_debug_enabled=0
ffly_mal_track=0
if [ $(bash find.bash "$1" "--debug-enabled") -eq 0 ]; then
	ffly_flags="$ffly_flags --debug-enabled"
	ffly_debug_enabled=1
fi

if [ $(bash find.bash "$1" "--mal-track") -eq 0 ]; then
	ffly_flags="$ffly_flags --mal-track"
	ffly_mal_track=1
fi

ffly_use_x11=0
ffly_use_xcb=0
if [ $(bash find.bash "$1" "--use-x11") -eq 0 ]; then
	ffly_flags="$ffly_flags --use-x11"
	ffly_use_x11=1
elif [ $(bash find.bash "$1" "--use-xcb") -eq 0 ]; then
	ffly_flags="$ffly_flags --use-xcb"
	ffly_use_xcb=1
else
	ffly_flags="$ffly_flags --using-x11"
	ffly_using_x11=1
fi

if command -v pkg-config > /dev/null 2>&1; then
	if $(pkg-config libpng --exists); then
		ffly_ld_flags="$ffly_ld_flags $(pkg-config libpng --libs-only-l)"
	fi
else
	if [ -f /usr/local/lib/x86_64-linux-gnu/libpng16.so ] ||
	[ -f /usr/lib/x86_64-linux-gnu/libpng16.so ] ||
	[ -f /lib/x86_64-linux-gnu/libpng16.so ]; then
		echo "fuck"
		ffly_ld_flags="$ffly_ld_flags -lpng16"
	fi
fi

if [ $ffly_force_cuda_use -eq 1 ]; then
	ffly_cl="cuda"
	ffly_ld_flags="$ffly_ld_flags -lcudart"
	ffly_inc_flags="$ffly_inc_flags -I$ffly_cuda_dir/$ffly_cuda_inc_dir_name"
	ffly_lib_flags="$ffly_lib_flags -L$ffly_cuda_dir/$ffly_cuda_lib_dir_name"
else
	echo "no computing library found for gpu.\n"
	return
fi

ffly_cflags="$ffly_cflags $ffly_inc_flags $ffly_lib_flags"
ffly_ccflags="$ffly_ccflags $ffly_cc_inc_flags $ffly_cc_lib_flags"
ffly_cxxflags="$ffly_cxxflags $ffly_cxx_inc_flags $ffly_cxx_lib_flags"

echo "target: $ffly_target"
echo "cl: $ffly_cl"
echo "arc: $ffly_arc"
echo "stdc: $ffly_stdc"
echo "stdcxx: $ffly_stdcxx"
echo "cc: $ffly_cc"
echo "cxx: $ffly_cxx"
echo "nvcc: $ffly_nvcc"
echo "inc-flags: $ffly_inc_flags"
echo "lib-flags: $ffly_lib_flags"
echo "ld-flags: $ffly_ld_flags"
echo "cc-inc-flags: $ffly_cc_inc_flags"
echo "cc-lib-flags: $ffly_cc_lib_flags"
echo "cc-ld-flags: $ffly_cc_ld_flags"
echo "cxx-inc-flags: $ffly_cxx_inc_flags"
echo "cxx-lib-flags: $ffly_cxx_lib_flags"
echo "cxx-ld-flags: $ffly_cxx_ld_flags"
echo "cflags: $ffly_cflags"
echo "ccflags: $ffly_ccflags"
echo "cxxflags: $ffly_cxxflags"
