. ./configure.sh CUDA
echo "compile $1 $2 $3 $4 -- $CXXFLAGS - $CUDART_INC $CUDART_LIBY"
make FFLY_TARGET=-D$3 EXTRA_DEFINES=$5
g++ -std=c++11 -DARC64 $CXXFLAGS -D$3 $5 -DUSING_CUDA -DUSING_X11 $CUDART_INC $CUDART_LIB -Iinc -Llib -Wall -o $1 $2 -l$4 $LDFLAGS -lX11 -lGL -lGLU -lglut -lfreetype -lm -lpulse -lpulse-simple
