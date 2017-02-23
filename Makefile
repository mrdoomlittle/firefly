SHELL :=/bin/bash
CURR_PATH=${CURDIR}
CXXFLAGS=-Ieint_t/inc -Iintlen/inc -Igetdigit/inc -Ito_string/inc -Istrcmb/inc -Lintlen/lib -Lgetdigit/lib -Lto_string/lib -Lstrcmb/lib
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
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) $(CUDA) -o src/ffly_client.o src/ffly_client.cpp

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

src/graphics/fill_pixmap.o: src/graphics/fill_pixmap.cu
	nvcc -c -std=c++11 $(CXXFLAGS) $(ARC) $(CUDA) -o src/graphics/fill_pixmap.o src/graphics/fill_pixmap.cu

src/tests/layering.o: src/tests/layering.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/tests/layering.o src/tests/layering.cpp

src/maths/rotate_point.o: src/maths/rotate_point.cpp
	g++ -c -std=c++11 $(CXXFLAGS) -Wall $(ARC) -o src/maths/rotate_point.o src/maths/rotate_point.cpp

required:
	cd intlen; make ARC64 EINT_T_INC=$(CURR_PATH)/eint_t/inc; cd ../;
	cd getdigit; make ARC64 EINT_T_INC=$(CURR_PATH)/eint_t/inc INTLEN_INC=$(CURR_PATH)/intlen/inc INTLEN_LIB=$(CURR_PATH)/intlen/lib; cd ../;
	cd to_string; make ECHAR_T=$(CURR_PATH)/echar_t/inc EINT_T_INC=$(CURR_PATH)/eint_t/inc GETDIGIT_INC=$(CURR_PATH)/getdigit/inc INTLEN_INC=$(CURR_PATH)/intlen/inc GETDIGIT_LIB=$(CURR_PATH)/getdigit/lib INTLEN_LIB=$(CURR_PATH)/intlen/lib; cd ../;
	cd  strcmb; make EINT_T_INC=$(CURR_PATH)/echar_t/inc EINT_T_INC=$(CURR_PATH)/eint_t/inc ARC=$(ARC); cd ../;

ffly_server: required src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o
	ld -r -o lib/ffly_server.o src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o

	ar rcs lib/libffly_server.a lib/ffly_server.o
	rm lib/ffly_server.o

	make move_headers;
	make move_libs;

ffly_client: required src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
	src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/tests/layering.o src/maths/rotate_point.o
	ld -r -o lib/ffly_client.o src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
	src/tests/layering.o src/maths/rotate_point.o

	ar rcs lib/libffly_client.a lib/ffly_client.o
	rm lib/ffly_client.o

	make move_headers;
	make move_libs;

move_libs:
	cp $(CURR_PATH)/getdigit/lib/*.a $(CURR_PATH)/lib
	cp $(CURR_PATH)/intlen/lib/*.a $(CURR_PATH)/lib
	cp $(CURR_PATH)/to_string/lib/*.a $(CURR_PATH)/lib
	cp $(CURR_PATH)/strcmb/lib/*.a $(CURR_PATH)/lib

move_headers:
	if ! [ -d $(CURR_PATH)/inc/firefly ]; then \
		mkdir $(CURR_PATH)/inc/firefly; \
	fi

	if ! [ -d $(CURR_PATH)/inc/firefly/graphics ]; then \
		mkdir $(CURR_PATH)/inc/firefly/graphics; \
	fi
	cp $(CURR_PATH)/src/graphics/*.hpp $(CURR_PATH)/inc/firefly/graphics

	if ! [ -d $(CURR_PATH)/inc/firefly/networking ]; then \
		mkdir $(CURR_PATH)/inc/firefly/networking; \
	fi
	cp $(CURR_PATH)/src/networking/*.hpp $(CURR_PATH)/inc/firefly/networking

	if ! [ -d $(CURR_PATH)/inc/firefly/types ]; then \
		mkdir $(CURR_PATH)/inc/firefly/types; \
	fi

	if ! [ -d $(CURR_PATH)/inc/firefly/tests ]; then \
		mkdir $(CURR_PATH)/inc/firefly/tests; \
	fi

	if ! [ -d $(CURR_PATH)/inc/firefly/maths ]; then \
		mkdir $(CURR_PATH)/inc/firefly/maths; \
	fi

	cp $(CURR_PATH)/src/maths/*.hpp $(CURR_PATH)/inc/firefly/maths

	cp $(CURR_PATH)/src/tests/*.hpp $(CURR_PATH)/inc/firefly/tests

	cp $(CURR_PATH)/src/types/*.hpp $(CURR_PATH)/inc/firefly/types

	cp $(CURR_PATH)/src/*.hpp $(CURR_PATH)/inc/firefly

	cp $(CURR_PATH)/eint_t/inc/*.hpp $(CURR_PATH)/inc
	cp $(CURR_PATH)/getdigit/inc/*.hpp $(CURR_PATH)/inc
	cp $(CURR_PATH)/intlen/inc/*.hpp $(CURR_PATH)/inc
	cp $(CURR_PATH)/to_string/inc/*.hpp $(CURR_PATH)/inc
	cp $(CURR_PATH)/strcmb/inc/*.hpp $(CURR_PATH)/inc

example_client: required src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
	src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o
	g++ -std=c++11 $(CXXFLAGS) $(CUDA) -Wall $(ARC) -o bin/example_client.exec example_client.cpp src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
	-lto_string -lgetdigit -lintlen -lstrcmb -lemu2d -lpulse -lpulse-simple -lpng16 -lcuda -lcudart -lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut

example_server: src/uni_worker.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o
	g++ -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/example_server.exec example_server.cpp \
	src/uni_worker.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o \
	-lpng16 -lboost_system -lemu2d -lOpenCL -lboost_filesystem -lpthread -lboost_thread

uni_worker: src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o
	g++ -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/uni_worker.exec src/uni_worker.cpp src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
	-lpng16 -lboost_system -lemu2d -lOpenCL -lboost_filesystem -lpthread -lboost_thread

skel_creator: src/graphics/x11_window.o
	nvcc -c -std=c++11 -DARC64 $(CUDA) -o draw_grid.o draw_grid.cu
	g++ -std=c++11 $(CUDA) -DARC64 -L/usr/local/lib -I/usr/local/include -o skel_creator.exec skel_creator.cpp draw_grid.o src/graphics/x11_window.o -lcuda -lcudart -lboost_system -lpthread -lboost_thread -lemu2d -lX11 -lGL -lGLU -lglut

clean:
	cd intlen; make clean; cd ../;
	cd getdigit; make clean; cd ../;
	cd to_string; make clean; cd ../;
	cd strcmb; make clean; cd ../;
	rm -f src/graphics/*.o
	rm -f src/networking/*.o
	rm -f src/maths/*.o
	rm -f src/tests/*.o
	rm -f src/*.o
	rm -f *.exec
	rm -f bin/*.exec
	rm -rf $(CURR_PATH)/inc/*
	rm -rf $(CURR_PATH)/lib/*
