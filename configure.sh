#!/bin/sh
CXX_IFLAGS="-Inibbles/inc -Itermio/inc -Imdlint/inc -Iintlen/inc -Igetdigit/inc -Ito_string/inc -Istr_cmb/inc -Iserializer/inc -Itagged_memory/inc -Iemu2d/inc -Iemu3d/inc -Iechar_t/inc"
CXX_LFLAGS="-Lnibbles/lib -Ltermio/lib -Lintlen/lib -Lgetdigit/lib -Lto_string/lib -Lstr_cmb/lib -Ltagged_memory/lib -Lemu2d/lib -Lemu3d/lib"
LDFLAGS="-lmdl-intlen -lmdl-getdigit -lmdl-to_string -lmdl-str_cmb -lmdl-emu2d -lmdl-emu3d -lboost_system -lboost_filesystem -lpthread -lboost_thread"
GPU_CL_TYPE=""
BOOST_PATH="/usr/local"
OPENCL_PATH="/usr"
CUDA_PATH="/usr"
CUDART_PATH="/usr/local/cuda-9.0"
CUDART_INC="-I$CUDART_PATH/include"
CUDART_LIB="-L$CUDART_PATH/lib64"
export CUDART_INC="$CUDART_INC"
export CUDART_LIC="$CUDART_LIB"
ARC=ARC$(getconf LONG_BIT)
FORCE_CUDA=0

ROOT_DIR=$(bash find.bash "$1" "--root-dir*")
if [ "$ROOT_DIR" = "1" ]; then
	ROOT_DIR=${PWD}
fi

if [ $(bash find.bash "$1" "--force-cuda") = "0" ]; then
	FORCE_CUDA=1
fi

if [ $(bash find.bash "$1" "--arc64") = "0" ]; then
	ARC="ARC64"
fi

if [ $(bash find.bash "$1" "--arc32") = "0" ]; then
	ARC="ARC32"
fi

if [ $(bash find.bash "$1" "--ffly-client") = "0" ]; then
	LDFLAGS="$LDFLAGS -lX11 -lGL -lGLU -lglut -lfreetype -lm -lpulse -lpulse-simple"
	CXX_IFLAGS="$CXX_IFLAGS -I/usr/include/freetype2"
elif [ $(bash find.bash "$1" "--ffly-server") = "0" ]; then
	echo "0"
elif [ $(bash find.bash "$1" "--ffly-studio") = "0" ]; then
	LDFLAGS="$LDFLAGS -lX11 -lGL -lGLU -lglut -lfreetype -lm -lpulse -lpulse-simple"
	CXX_IFLAGS="$CXX_IFLAGS -I/usr/include/freetype2"
elif [ $(bash find.bash "$1" "--ffly-worker") = "0" ]; then
	echo "0"
elif [ $(bash find.bash "$1" "--ffly-test") = "0" ]; then
	LDFLAGS="$LDFLAGS -lX11 -lGL -lGLU -lglut -lX11-xcb -lxcb -lxcb-icccm -lpulse -lpulse-simple -lasound"
else
	echo "sorry but we need to know what the target is :( e.g. --ffly-client, --ffly-studio etc"
	return
fi

FFLY_ARGS=
if [ $(bash find.bash "$1" "--with-room-manager") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --with-room-manager"
fi

if [ $(bash find.bash "$1" "--with-obj-manager") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --with-obj-manager"
fi

if [ $(bash find.bash "$1" "--rm-layering") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --rm-layering"
fi

if [ $(bash find.bash "$1" "--with-uni-manager") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --with-uni-manager"
fi

if [ $(bash find.bash "$1" "--debug-enabled") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --debug-enabled"
fi

if [ $(bash find.bash "$1" "--with-mem-tracker") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --with-mem-tracker"
fi

if [ $(bash find.bash "$1" "--using-x11") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --using-x11"
elif [ $(bash find.bash "$1" "--using-xcb") = "0" ]; then
	FFLY_ARGS="$FFLY_ARGS --using-xcb"
else
	FFLY_ARGS="$FFLY_ARGS --using-x11"
fi

export FFLY_ARGS="$FFLY_ARGS"

if ! [ -f "$BOOST_PATH/lib/libboost_system.a" ]; then
	if ! [ -f "$BOOST_PATH/lib/libboost_system.so" ]; then
		if ! $(pkg-config libboost_system --exists); then
			echo "you will need to install boost system to continue"
			return
		fi
	fi
fi

if ! [ -f "$BOOST_PATH/lib/libboost_filesystem.a" ]; then
    if ! [ -f "$BOOST_PATH/lib/libboost_filesystem.so" ]; then
		if ! $(pkg-config libboost_filesystem --exists); then
        	echo "you will need to install boost system to continue"
			return
		fi
    fi
fi

if ! [ -f "$BOOST_PATH/lib/libboost_thread.a" ]; then
    if ! [ -f "$BOOST_PATH/lib/libboost_thread.so" ]; then
		if ! $(pkg-config libboost_thread --exists); then
        	echo "you will need to install boost system to continue"
			return
		fi
    fi
fi

# check if libpng is installed
if $(pkg-config libpng --exists); then
	LDFLAGS="$LDFLAGS $(pkg-config libpng --libs-only-l)"
fi

if [ $FORCE_CUDA -eq 1 ]; then
	GPU_CL_TYPE="-DUSING_CUDA"
	LDFLAGS="$LDFLAGS -lcuda -lcudart"
	CXX_IFLAGS="$CXX_IFLAGS -I$CUDART_PATH/include"
#	CXXFLAGS="$CXXFLAGS $CUDART_PATH/include"
else
# check what type of cl library we are going to use e.g. opengl or cuda etc...
if [ -f "$OPENCL_PATH/lib/x86_64-linux-gnu/libOpenCL.so.1" ]; then
	GPU_CL_TYPE="-DUSING_OPENCL"
	CXX_IFLAGS="$CXX_IFLAGS -D__USING_OPENCL"
	LDFLAGS="$LDFLAGS -lOpenCL"
	CXX_LFLAGS="$CXX_LFLAGS -L/usr/local/lib/x86_64/sdk"
elif [ -f "$CUDA_PATH/lib/x86_64-linux-gnu/libcuda.so" ]; then
	if [ -d "$CUDART_PATH" ]; then
		GPU_CL_TYPE="-DUSING_CUDA"
		CXX_IFLAGS="$CXX_IFLAGS -D__USING_CUDA"
		LDFLAGS="$LDFLAGS -lcudart"
#		CXXFLAGS="$CXXFLAGS $CUDART_PATH/include $CUDART_PATH/lib64"
	else
		echo "cudart dir does not exist"
		return
	fi
else
	echo "no cl library found"
fi
fi

CXXFLAGS="$CXX_IFLAGS $CXX_LFLAGS"
C_IFLAGS="$CXX_IFLAGS"
export CXX_IFLAGS="$CXX_IFLAGS"
export CXX_LFLAGS="$CXX_LFLAGS"
export CUDART_INC="$CUDART_INC"
export CUDART_LIB="$CUDART_LIB"

export ARC="$ARC"
#export CXXFLAGS="$CXXFLAGS $ARC"

export CXXFLAGS="$CXXFLAGS $GPU_CL_TYPE -D__$ARC"
export C_IFLAGS="$C_IFLAGS"

export LDFLAGS="$LDFLAGS"
export GPU_CL_TYPE="$GPU_CL_TYPE"
export ROOT_DIR="$ROOT_DIR"
