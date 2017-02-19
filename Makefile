CXXFLAGS=-Ieint_t/inc
LDFLAGS=
LLFLAGS=
ARC=-DARC64
CUDA=-I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64
src/graphics/x11_window.o: src/graphics/x11_window.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/graphics/x11_window.o src/graphics/x11_window.cpp

src/graphics/png_loader.o: src/graphics/png_loader.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/graphics/png_loader.o src/graphics/png_loader.cpp

src/networking/tcp_server.o: src/networking/tcp_server.cpp
	g++ -c -std=c++11 $(CXXFLAGS)-Wall $(ARC) -o src/networking/tcp_server.o src/networking/tcp_server.cpp

src/networking/tcp_client.o: src/networking/tcp_client.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/networking/tcp_client.o src/networking/tcp_client.cpp

src/networking/udp_server.o: src/networking/udp_server.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/networking/udp_server.o src/networking/udp_server.cpp

src/networking/udp_client.o: src/networking/udp_client.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/networking/udp_client.o src/networking/udp_client.cpp

src/ffly_client.o: src/ffly_client.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/ffly_client.o src/ffly_client.cpp

src/ffly_server.o: src/ffly_server.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/ffly_server.o src/ffly_server.cpp

src/graphics/draw_rect.o: src/graphics/draw_rect.cu
	nvcc -c -std=c++11 $(CXXFLAGS) $(ARC) $(CUDA) -o src/graphics/draw_rect.o src/graphics/draw_rect.cu

src/graphics/draw_skelmap.o: src/graphics/draw_skelmap.cu
	nvcc -c -std=c++11 $(CXXFLAGS) $(ARC) $(CUDA) -o src/graphics/draw_skelmap.o src/graphics/draw_skelmap.cu

src/graphics/skelmap_loader.o: src/graphics/skelmap_loader.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/graphics/skelmap_loader.o src/graphics/skelmap_loader.cpp

src/asset_manager.o: src/asset_manager.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/asset_manager.o src/asset_manager.cpp

src/graphics/draw_pixmap.o: src/graphics/draw_pixmap.cu
	nvcc -c -std=c++11 $(CXXFLAGS) $(ARC) $(CUDA) -o src/graphics/draw_pixmap.o src/graphics/draw_pixmap.cu

example_client: src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
	src/asset_manager.o src/graphics/draw_pixmap.o
	g++ -std=c++11 $(CXXFLAGS) $(CUDA) -Wall $(ARC) -o bin/example_client.exec example_client.cpp src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o \
	-lemu2d -lpulse -lpulse-simple -lpng16 -lcuda -lcudart -lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut

example_server: src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o
	g++ -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/example_server.exec example_server.cpp \
	src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o \
	-lpng16 -lboost_system -lemu2d -lOpenCL -lboost_filesystem -lpthread -lboost_thread

skel_creator: src/graphics/x11_window.o
	nvcc -c -std=c++11 -DARC64 $(CUDA) -o draw_grid.o draw_grid.cu
	g++ -std=c++11 $(CUDA) -DARC64 -L/usr/local/lib -I/usr/local/include -o skel_creator.exec skel_creator.cpp draw_grid.o src/graphics/x11_window.o -lcuda -lcudart -lboost_system -lpthread -lboost_thread -lemu2d -lX11 -lGL -lGLU -lglut

clean:
	rm -f src/graphics/*.o
	rm -f src/networking/*.o
	rm -f src/*.o
	rm -f *.exec
	rm -f bin/*.exec
