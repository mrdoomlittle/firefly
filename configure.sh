#!/bin/sh
CXX_IFLAGS="-Ieint_t/inc -Iintlen/inc -Igetdigit/inc -Ito_string/inc -Istrcmb/inc -Iserializer/inc"
CXX_LFLAGS="-Lintlen/lib -Lgetdigit/lib -Lto_string/lib -Lstrcmb/lib"
CXXFLAGS="$CXX_IFLAGS $CXX_LFLAGS"
LDFLAGS="-lintlen -lgetdigit -lto_string -lstrcmb -lboost_system -lboost_filesystem -lpthread -lboost_thread"
GPU_CL_TYPE=""
BOOST_PATH="/usr/local"
OPENCL_PATH="/usr"
CUDA_PATH="/usr"
CUDART_PATH="/usr/local/cuda-8.0"
CUDART_INC="-I$CUDART_PATH/include"
CUDART_LIB="-L$CUDART_PATH/lib64"
export CUDART_INC="$CUDART_INC"
export CUDART_LIC="$CUDART_LIB"
ARC="-DARC32"
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

if [ "$1" = "CUDA" ]; then 
	GPU_CL_TYPE="-DUSING_CUDA"
	LDFLAGS="$LDFLAGS -lcuda -lcudart"
#	CXXFLAGS="$CXXFLAGS $CUDART_PATH/include"
else
# check what type of cl library we are going to use e.g. opengl or cuda etc...
if [ -f "$OPENCL_PATH/lib/x86_64-linux-gnu/libOpenCL.so.1" ]; then
	GPU_CL_TYPE="-DUSING_OPENCL"
	LDFLAGS="$LDFLAGS -lOpenCL"
elif [ -f "$CUDA_PATH/lib/x86_64-linux-gnu/libcuda.so" ]; then
	if [ -d "$CUDART_PATH" ]; then
		GPU_CL_TYPE="-DUSING_CUDA"
		LDFLAGS="$LDFLAGS -lcuda -lcudart"
#		CXXFLAGS="$CXXFLAGS $CUDART_PATH/include $CUDART_PATH/lib64"
	else
		echo "cudart dir does not exist"
		return
	fi
else
	echo "no cl library found"
fi
fi

export CXX_IFLAGS="$CXX_IFLAGS"
export CXX_LFLAGS="$CXX_LFLAGS"

export ARC="-DARC$(getconf LONG_BIT)"
#export CXXFLAGS="$CXXFLAGS $ARC"

export CXXFLAGS="$CXXFLAGS $GPU_CL_TYPE $ARC"
export LDFLAGS="$LDFLAGS"
export GPU_CL_TYPE="$GPU_CL_TYPE"
