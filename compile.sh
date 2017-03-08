. ./configure.sh CUDA;
make FFLY_TARGET=-DFFLY_CLIENT;
echo "compile $1 $2 -- $CXXFLAGS"
g++ -std=c++11 $CXXFLAGS -DFFLY_CLIENT -DUSING_CUDA -DUSING_X11 $CUDART_INC $CUDART_LIB -Iinc -Llib -Wall -o $1 $2 -lffly_client $LDFLAGS -lX11 -lGL -lGLU -lglut
