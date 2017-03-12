SHELL := /bin/bash
CURR_DIR=${CURDIR}
#CXXFLAGS=@echo ${CXXFLAGS}#-DDEBUG_ENABLED -Ieint_t/inc -Iintlen/inc -Igetdigit/inc -Ito_string/inc -Istrcmb/inc -Iserializer/inc -Lintlen/lib -Lgetdigit/lib -Lto_string/lib -Lstrcmb/lib
#LDFLAGS=
#ARC=-DARC64
#CUDA=-I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64

#ifndef $(FFLY_TARGET)
#	FFLY_TARGET=-DFFLY_NONE
#endif

#LIBRARY_OBJS=intlen/src/intlen.o to_string/src/to_string.o strcmb/src/strcmb.o getdigit/src/getdigit.
FFLY_OBJECTS=

#tt:
#	echo "$(CUDA)"

ifeq ($(FFLY_TARGET), -DFFLY_WORKER)
	FFLY_OBJECTS += src/networking/tcp_client.o src/networking/udp_client.o src/graphics/png_loader.o src/memory/alloc_pixmap.o

else ifeq ($(FFLY_TARGET), -DFFLY_SERVER)
	FFLY_OBJECTS += src/ffly_server.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/graphics/png_loader.o \
	src/worker_manager.o src/memory/alloc_pixmap.o src/graphics/draw_pixmap.clo

else ifeq ($(FFLY_TARGET), -DFFLY_CLIENT)
	FFLY_OBJECTS += src/ffly_client.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/x11_window.o src/graphics/png_loader.o \
	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o \
	src/tests/layering.o src/maths/rotate_point.o src/graphics/scale_pixmap.o src/graphics/fill_pixmap.o src/memory/alloc_pixmap.o src/graphics/window.o \
	src/layer_manager.o src/obj_manager.o src/maths/cal_dist.o src/maths/squar_rt.o src/maths/squar.o src/gravy_manager.o src/flip_direction.o
	LDFLAGS += -lX11 -lGL -lGLU -lglut
else ifeq ($(FFLY_TARGET), -DFFLY_STUDIO)
	FFLY_OBJECTS += src/graphics/x11_window.o src/memory/alloc_pixmap.o src/graphics/window.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
	src/gui/btn_manager.o src/graphics/draw_skelmap.o src/graphics/draw_bitmap.o #src/ffly_studio.o
	LDFLAGS += -lX11 -lGL -lGLU -lglut -lfreetype -lm
	CXX_IFLAGS += -I/usr/include/freetype2 $(CUDART_INC)
else
	FFLY_OBJECTS=
	FFLY_TARGET=-DFFLY_NONE
endif

ifeq ($(GPU_CL_TYPE), -DUSING_OPENCL)
	CXXFLAGS += -L/usr/local/lib/x86_64/sdk
	FFLY_OBJECTS += src/opencl_helper.o
endif

## core memory stuff
FFLY_OBJECTS += src/memory/mem_alloc.o src/memory/mem_free.o #src/memory/alloc_pixmap.o
CXXFLAGS += $(FFLY_TARGET)

ifeq ($(FFLY_TARGET), -DFFLY_WORKER)
all: ffly_worker
else ifeq ($(FFLY_TARGET), -DFFLY_SERVER)
all: ffly_server
else ifeq ($(FFLY_TARGET), -DFFLY_CLIENT)
all: ffly_client
else ifeq ($(FFLY_TARGET), -DFFLY_STUDIO)
all: ffly_studio
endif

# using x11 for now

FFLY_DEFINES=$(GPU_CL_TYPE) $(ARC) -DUSING_X11

src/graphics/x11_window.o: src/graphics/x11_window.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/x11_window.o src/graphics/x11_window.cpp

src/graphics/png_loader.o: src/graphics/png_loader.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/png_loader.o src/graphics/png_loader.cpp

src/networking/tcp_server.o: src/networking/tcp_server.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/tcp_server.o src/networking/tcp_server.cpp

src/networking/tcp_client.o: src/networking/tcp_client.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/tcp_client.o src/networking/tcp_client.cpp

src/networking/udp_server.o: src/networking/udp_server.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/udp_server.o src/networking/udp_server.cpp

src/networking/udp_client.o: src/networking/udp_client.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/udp_client.o src/networking/udp_client.cpp

src/ffly_worker.o: src/uni_worker.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_worker.o src/uni_worker.cpp

src/ffly_server.o: src/ffly_server.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_server.o src/ffly_server.cpp

src/ffly_client.o: src/ffly_client.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) $(CUDART_INC) -o src/ffly_client.o src/ffly_client.cpp

src/graphics/draw_rect.o: src/graphics/draw_rect.cu
	nvcc -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_rect.o src/graphics/draw_rect.cu

src/graphics/draw_skelmap.o: src/graphics/draw_skelmap.cu
	nvcc -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_skelmap.o src/graphics/draw_skelmap.cu

src/graphics/skelmap_loader.o: src/graphics/skelmap_loader.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/skelmap_loader.o src/graphics/skelmap_loader.cpp

src/asset_manager.o: src/asset_manager.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/asset_manager.o src/asset_manager.cpp

src/graphics/draw_pixmap.o: src/graphics/draw_pixmap.cu
	nvcc -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_pixmap.o src/graphics/draw_pixmap.cu

src/graphics/fill_pixmap.o: src/graphics/fill_pixmap.cu
	nvcc -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/fill_pixmap.o src/graphics/fill_pixmap.cu

src/tests/layering.o: src/tests/layering.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/tests/layering.o src/tests/layering.cpp

src/maths/rotate_point.o: src/maths/rotate_point.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/maths/rotate_point.o src/maths/rotate_point.cpp

src/graphics/scale_pixmap.o: src/graphics/scale_pixmap.cu
	nvcc -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/scale_pixmap.o src/graphics/scale_pixmap.cu

src/opencl_helper.o: src/opencl_helper.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/opencl_helper.o src/opencl_helper.cpp

src/worker_manager.o: src/worker_manager.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/worker_manager.o src/worker_manager.cpp

src/memory/alloc_pixmap.o:src/memory/alloc_pixmap.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/alloc_pixmap.o src/memory/alloc_pixmap.cpp

src/graphics/draw_pixmap.clo: src/graphics/draw_pixmap.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_pixmap.clo src/graphics/draw_pixmap.cpp

src/memory/mem_alloc.o: src/memory/mem_alloc.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_alloc.o src/memory/mem_alloc.cpp

src/memory/mem_free.o: src/memory/mem_free.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_free.o src/memory/mem_free.cpp

src/graphics/window.o: src/graphics/window.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/window.o src/graphics/window.cpp

src/layer_manager.o: src/layer_manager.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/layer_manager.o src/layer_manager.cpp

src/obj_manager.o: src/obj_manager.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/obj_manager.o src/obj_manager.cpp

src/maths/cal_dist.o: src/maths/cal_dist.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/maths/cal_dist.o src/maths/cal_dist.cpp

src/maths/squar_rt.o: src/maths/squar_rt.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/maths/squar_rt.o src/maths/squar_rt.cpp

src/maths/squar.o: src/maths/squar.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/maths/squar.o src/maths/squar.cpp

src/gravy_manager.o: src/gravy_manager.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/gravy_manager.o src/gravy_manager.cpp

src/flip_direction.o: src/flip_direction.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/flip_direction.o src/flip_direction.cpp

src/ffly_studio.o: src/ffly_studio.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_studio.o src/ffly_studio.cpp

src/gui/btn_manager.o: src/gui/btn_manager.cpp
	g++ -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/gui/btn_manager.o src/gui/btn_manager.cpp

src/graphics/draw_bitmap.o: src/graphics/draw_bitmap.cu
	nvcc -c -std=c++11 $(CXX_IFLAGS) $(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_bitmap.o src/graphics/draw_bitmap.cu

relocate_headers:
	if ! [ -d $(CURR_DIR)/inc/firefly ]; then \
		mkdir $(CURR_DIR)/inc/firefly; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/graphics ]; then \
		mkdir $(CURR_DIR)/inc/firefly/graphics; \
	fi
	cp $(CURR_DIR)/src/graphics/*.hpp $(CURR_DIR)/inc/firefly/graphics

	if ! [ -d $(CURR_DIR)/inc/firefly/networking ]; then \
		mkdir $(CURR_DIR)/inc/firefly/networking; \
	fi
	cp $(CURR_DIR)/src/networking/*.hpp $(CURR_DIR)/inc/firefly/networking

	if ! [ -d $(CURR_DIR)/inc/firefly/types ]; then \
		mkdir $(CURR_DIR)/inc/firefly/types; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/tests ]; then \
		mkdir $(CURR_DIR)/inc/firefly/tests; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/maths ]; then \
		mkdir $(CURR_DIR)/inc/firefly/maths; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/system ]; then \
		mkdir $(CURR_DIR)/inc/firefly/system; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/memory ]; then \
		mkdir $(CURR_DIR)/inc/firefly/memory; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/system ]; then \
		mkdir $(CURR_DIR)/inc/firefly/system; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/gui ]; then \
		mkdir $(CURR_DIR)/inc/firefly/gui; \
	fi

	cp $(CURR_DIR)/src/gui/*.h $(CURR_DIR)/inc/firefly/gui
	cp $(CURR_DIR)/src/gui/*.hpp $(CURR_DIR)/inc/firefly/gui

	cp $(CURR_DIR)/src/system/*.hpp $(CURR_DIR)/inc/firefly/system

	cp $(CURR_DIR)/src/memory/*.h $(CURR_DIR)/inc/firefly/memory
	cp $(CURR_DIR)/src/memory/*.hpp $(CURR_DIR)/inc/firefly/memory

	cp $(CURR_DIR)/src/system/*.h $(CURR_DIR)/inc/firefly/system

	cp $(CURR_DIR)/src/maths/*.h $(CURR_DIR)/inc/firefly/maths
	cp $(CURR_DIR)/src/maths/*.hpp $(CURR_DIR)/inc/firefly/maths

	cp $(CURR_DIR)/src/tests/*.hpp $(CURR_DIR)/inc/firefly/tests

	cp $(CURR_DIR)/src/types/*.h $(CURR_DIR)/inc/firefly/types
	cp $(CURR_DIR)/src/types/*.hpp $(CURR_DIR)/inc/firefly/types

	cp $(CURR_DIR)/src/*.h $(CURR_DIR)/inc/firefly
	cp $(CURR_DIR)/src/*.hpp $(CURR_DIR)/inc/firefly

	cp $(CURR_DIR)/eint_t/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/getdigit/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/intlen/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/to_string/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/strcmb/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/serializer/inc/*.hpp $(CURR_DIR)/inc

# core libraries that are needed by design
libraries:
	cd intlen; make ARC64 EINT_T_INC=$(CURR_DIR)/eint_t/inc; cd ../;
	cd getdigit; make ARC64 EINT_T_INC=$(CURR_DIR)/eint_t/inc INTLEN_INC=$(CURR_DIR)/intlen/inc INTLEN_LIB=$(CURR_DIR)/intlen/lib; cd ../;
	cd to_string; make ECHAR_T=$(CURR_DIR)/echar_t/inc EINT_T_INC=$(CURR_DIR)/eint_t/inc GETDIGIT_INC=$(CURR_DIR)/getdigit/inc INTLEN_INC=$(CURR_DIR)/intlen/inc GETDIGIT_LIB=$(CURR_DIR)/getdigit/lib INTLEN_LIB=$(CURR_DIR)/intlen/lib; cd ../;
	cd strcmb; make EINT_T_INC=$(CURR_DIR)/echar_t/inc EINT_T_INC=$(CURR_DIR)/eint_t/inc ARC=$(ARC); cd ../;

ffly_worker: libraries $(FFLY_OBJECTS)
	ld -r -o lib/ffly_worker.o $(FFLY_OBJECTS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_worker.a lib/ffly_worker.o
	make relocate_headers

ffly_server: libraries $(FFLY_OBJECTS)
	ld -r -o lib/ffly_server.o $(FFLY_OBJECTS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_server.a lib/ffly_server.o
	make relocate_headers

ffly_client: libraries $(FFLY_OBJECTS)
	ld -r -o lib/ffly_client.o $(FFLY_OBJECTS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_client.a lib/ffly_client.o
	make relocate_headers

ffly_studio: libraries $(FFLY_OBJECTS) src/ffly_studio.o
	ld -r -o lib/ffly_studio.o $(FFLY_OBJECTS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_studio.a lib/ffly_studio.o
	make relocate_headers
	#g++ -std=c++11 -Iinc -Llib -Wall -o bin/ffly_studio src/ffly_studio.o -lffly_studio

#ffly_server: required src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/opencl_helper.o
#	ld -r -o lib/ffly_server.o src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/opencl_helper.o
#
#	ar rcs lib/libffly_server.a lib/ffly_server.o
#	rm lib/ffly_server.o
#
#	make move_headers;
#	make move_libs;

#ffly_client: required src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
#	src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/tests/layering.o src/maths/rotate_point.o src/graphics/scale_pixmap.o
#	ld -r -o lib/ffly_client.o src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
#	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
#	src/tests/layering.o src/maths/rotate_point.o src/graphics/scale_pixmap.o
#
#	ar rcs lib/libffly_client.a lib/ffly_client.o
#	rm lib/ffly_client.o
#
#	make move_headers;
#	make move_libs;

#move_libs:#
#	cp $(CURR_DIR)/getdigit/lib/*.a $(CURR_DIR)/lib
#	cp $(CURR_DIR)/intlen/lib/*.a $(CURR_DIR)/lib
#	cp $(CURR_DIR)/to_string/lib/*.a $(CURR_DIR)/lib
#	cp $(CURR_DIR)/strcmb/lib/*.a $(CURR_DIR)/lib



#src/ffly_memory.o: src/memory/mem_alloc.o src/memory/mem_free.o src/memory/alloc_pixmap.o
#	ld -r -o src/ffly_memory.o src/memory/mem_alloc.o src/memory/mem_free.o src/memory/alloc_pixmap.o

#example_client: required src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
#	src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/ffly_memory.o
#	g++ -std=c++11 $(CXXFLAGS) -DUSING_CUDA -DFFLY_CLIENT $(CUDA) -Wall $(ARC) -o bin/example_client.exec example_client.cpp src/ffly_client.o src/graphics/x11_window.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
#	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/ffly_memory.o \
#	-lto_string -lgetdigit -lintlen -lstrcmb -lemu2d -lpulse -lpulse-simple -lpng16 -lcuda -lcudart -lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut

#example_server: src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o \
#	src/opencl_helper.o src/worker_manager.o src/memory/alloc_pixmap.o src/graphics/draw_pixmap.clo
#	g++ -std=c++11 $(CXXFLAGS) -DUSING_OPENCL -DFFLY_SERVER -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/example_server.exec example_server.cpp \
#	src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/opencl_helper.o src/worker_manager.o src/memory/alloc_pixmap.o \
#	src/graphics/draw_pixmap.clo -lpng16 -lboost_system -lemu2d -lOpenCL -lboost_filesystem -lpthread -lboost_thread

example_worker:
	make FFLY_TARGET=-DFFLY_WORKER;
	g++ -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -DFFLY_WORKER -Iinc -Llib -Wall -o bin/example_worker.exec src/uni_worker.cpp -lffly_worker $(LDFLAGS)

example_server:
	make FFLY_TARGET=-DFFLY_SERVER;
	g++ -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -DFFLY_SERVER -Iinc -Llib -Wall -o bin/example_server.exec example_server.cpp -lffly_server $(LDFLAGS)

example_client:
	make FFLY_TARGET=-DFFLY_CLIENT;
	g++ -std=c++11 $(CXXFLAGS) -DFFLY_CLIENT $(CUDART_INC) $(CUDART_LIB) -Iinc -Llib -Wall -o bin/example_client.exec example_client.cpp -lffly_client $(LDFLAGS) -lX11 -lGL -lGLU -lglut

#example_worker:
#	make FFLY_TARGET=-DFFLY_WORKER;

#uni_worker: src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/ffly_memory.o
#	g++ -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/uni_worker.exec src/uni_worker.cpp src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
#	src/ffly_memory.o -lpng16 -lboost_system -lOpenCL -lboost_filesystem -lpthread -lboost_thread

skel_creator: src/graphics/x11_window.o
	nvcc -c -std=c++11 -DARC64 $(CUDA) -o draw_grid.o draw_grid.cu
	g++ -std=c++11 $(CUDA) -DARC64 -L/usr/local/lib -I/usr/local/include -o skel_creator.exec skel_creator.cpp draw_grid.o src/graphics/x11_window.o -lcuda -lcudart -lboost_system -lpthread -lboost_thread -lemu2d -lX11 -lGL -lGLU -lglut

clean:
	cd intlen; make clean; cd ../;
	cd getdigit; make clean; cd ../;
	cd to_string; make clean; cd ../;
	cd strcmb; make clean; cd ../;

	rm -f src/gui/*.o src/memory/*.o src/graphics/*.o src/networking/*.o src/maths/*.o src/tests/*.o src/*.o *.exec #bin/*.exec
	rm -rf $(CURR_DIR)/inc/* $(CURR_DIR)/lib/*
