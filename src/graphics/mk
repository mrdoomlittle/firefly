USING=
dep: clean
	g++ -c -std=c++11 ../memory/alloc_pixmap.cpp
	g++ -c -std=c++11 ../memory/mem_alloc.cpp
	g++ -c -std=c++11 ../memory/mem_free.cpp

	g++ -c -std=c++11 -I/usr/local/include -DUSING_CUDA -D$(USING) -o window.o window.cpp
	nvcc -c -std=c++11 -I/usr/local/include -DUSING_CUDA -o draw_pixmap.o draw_pixmap.cu
	nvcc -c -std=c++11 -I/usr/local/include -DUSING_CUDA -o fill_pixmap.o fill_pixmap.cu
#	rm -f draw_pixmap.o fill_pixmap.o;
#	cd ../../; make src/graphics/draw_pixmap.o && make src/graphics/fill_pixmap.o; cd src/graphics;


OBJS=alloc_pixmap.o mem_alloc.o mem_free.o window.o draw_pixmap.o fill_pixmap.o
x11: dep
	g++ -std=c++11 -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -DUSING_CUDA -DUSING_X11 -o main x11_window.cpp $(OBJS) -lX11 -lGL -lGLU -lglut -lboost_system -lboost_filesystem -lpthread -lboost_thread -lcuda -lcudart
xcb: dep
	g++ -std=c++11 -I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64 -DUSING_CUDA -DUSING_XCB -o main xcb_window.cpp $(OBJS) -lxcb -lxcb-icccm -lX11 -lGL -lGLU -lglut -lX11-xcb -lcuda -lboost_system -lboost_filesystem -lpthread -lboost_thread -lcudart
clean:
	cd ../../; make clean; cd src/graphics;
