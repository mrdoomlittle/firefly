nvcc -c -std=c++11 -o test.cu.o test.cu
g++ -std=c++11 -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -o test test.cpp test.cu.o -lpthread -lcuda -lcudart
