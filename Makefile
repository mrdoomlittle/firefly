SHELL := /bin/bash
CURR_DIR=${CURDIR}
#CXXFLAGS=@echo ${CXXFLAGS}#-DDEBUG_ENABLED -Imdlint/inc -Iintlen/inc -Igetdigit/inc -Ito_string/inc -Istrcmb/inc -Iserializer/inc -Lintlen/lib -Lgetdigit/lib -Lto_string/lib -Lstrcmb/lib
#LDFLAGS=
#ARC=-DARC64
#CUDA=-I/usr/local/cuda-8.0/include -L/usr/local/cuda-8.0/lib64

#ifndef $(FFLY_TARGET)
#	FFLY_TARGET=-DFFLY_NONE
#endif

#LIBRARY_OBJS=intlen/src/intlen.o to_string/src/to_string.o strcmb/src/strcmb.o getdigit/src/getdigit.
FFLY_OBJS=
FFLY_WINDOW=
EXTRA_DEFINES=

CXX_VERSION=c++11
C_VERSION=c11
ifeq ($(shell bash find.bash "$(FFLY_ARGS)" "--with-uni-manager"), 0)
 FFLY_OBJS+= src/uni_manager.o src/chunk_manager.o src/chunk_keeper.o src/data/uni_dlen_val.o
endif

ifeq ($(shell bash find.bash "$(FFLY_ARGS)" "--with-obj-manager"), 0)
 FFLY_OBJS+= src/obj_manager.o src/gravy_manager.o
endif

ifeq ($(FFLY_TARGET), FFLY_SERVER)
 FFLY_OBJS += src/ffly_server.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/graphics/png_loader.o \
 src/worker_manager.o src/memory/alloc_pixmap.o src/graphics/draw_pixmap.clo src/player_manager.o src/player_handler.o src/worker_handler.o

else ifeq ($(FFLY_TARGET), FFLY_CLIENT)
 FFLY_OBJS += src/ffly_client.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/png_loader.o \
 src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/draw_pixmap.clo \
 src/tests/layering.o src/maths/rotate_point.o src/graphics/scale_pixmap.o src/graphics/fill_pixmap.o src/memory/alloc_pixmap.o src/graphics/window.o \
 src/layer_manager.o src/maths/cal_dist.o src/system/time_stamp.o src/room_manager.o \
 src/gui/btn_manager.o src/system/event.o src/graphics/draw_bitmap.o src/font.o src/system/task_handle.o src/system/task_worker.o \
 src/ui/camera.o src/graphics/crop_pixmap.o src/entity_manager.o src/system/smem_buff.o
else ifeq ($(FFLY_TARGET), FFLY_STUDIO)
 FFLY_OBJS += src/skel_creator.o src/graphics/draw_grid.o src/ffly_audio.o src/memory/alloc_pixmap.o src/graphics/window.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
 src/gui/btn_manager.o src/graphics/draw_skelmap.o src/graphics/draw_bitmap.o src/pulse_audio.o src/maths/rotate_point.o \
 src/graphics/png_loader.o src/room_manager.o src/asset_manager.o src/system/time_stamp.o  src/graphics/draw_rect.o \
 src/gui/wd_frame.o src/gui/window.o src/data/scale_pixmap.o src/graphics/draw_pixmap.clo src/system/task_handle.o src/system/task_worker.o \
 src/system/smem_buff.o src/system/event.o
 LDFLAGS += -lX11 -lGL -lGLU -lglut -lfreetype -lm -lpulse -lpulse-simple
 CXX_IFLAGS += -I/usr/include/freetype2 $(CUDART_INC)
else ifeq ($(FFLY_TARGET), FFLY_WORKER)
 FFLY_OBJS += src/uni_worker.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/png_loader.o src/memory/alloc_pixmap.o
else ifeq ($(FFLY_TARGET), FFLY_TEST)
 FFLY_OBJS+= src/graphics/crop_pixmap.o src/memory/alloc_pixmap.o #src/system/task_handle.o src/system/task_worker.o src/graphics/crop_pixmap.o
else
 FFLY_OBJS=
 FFLY_TARGET=FFLY_NONE
endif

ifeq ($(GPU_CL_TYPE), -DUSING_OPENCL)
 CXXFLAGS+= -L/usr/local/lib/x86_64/sdk
 FFLY_OBJS+= src/opencl_helper.o
else ifeq ($(GPU_CL_TYPE), -DUSING_CUDA)
 FFLY_OBJS+= src/cuda_helper.o
endif

FFLY_OBJS+= src/firefly.o
# lower level core - things that are needed for things to work
# maths
FFLY_OBJS+= src/maths/round.o src/maths/ceil.o src/maths/floor.o src/maths/sq.o
# data
FFLY_OBJS+= src/data/swp.o src/data/mem_ncpy.o
# system
FFLY_OBJS+= src/system/buff.o src/system/vec.o src/system/time.o src/system/config.o src/system/errno.o src/system/io.o
# graphics
FFLY_OBJS+= src/ffly_graphics.o
# memory
FFLY_OBJS+= src/ffly_memory.o src/memory/mem_alloc.o src/memory/mem_alloc.co src/memory/mem_free.co src/memory/mem_realloc.co #src/memory/alloc_pixmap.o

CXXFLAGS += $(FFLY_TARGET)

ifeq ($(shell bash find.bash "$(FFLY_ARGS)" "--with-mem-tracker"), 0)
	FFLY_OBJS += src/system/mem_tracker.o
endif

ifneq ($(FFLY_TARGET), $(filter $(FFLY_TARGET), FFLY_SERVER FFLY_WORKER FFLY_TEST))
ifeq ($(FFLY_WINDOW), -DUSING_X11)
 FFLY_OBJS += src/graphics/x11_window.o
else ifeq ($(FFLY_WINDOW), -DUSING_XCB)
 FFLY_OBJS += src/graphics/xcb_window.o
endif
endif

ifeq ($(FFLY_TARGET), FFLY_WORKER)
all: ffly_worker
else ifeq ($(FFLY_TARGET), FFLY_SERVER)
all: ffly_server
else ifeq ($(FFLY_TARGET), FFLY_CLIENT)
all: ffly_client
else ifeq ($(FFLY_TARGET), FFLY_STUDIO)
all: ffly_studio
else ifeq ($(FFLY_TARGET), FFLY_TEST)
all: ffly_test
endif

FFLY_DEFINES=-D__GCOMPUTE_GPU -D__GCOMPUTE_CPU $(GPU_CL_TYPE) $(ARC) $(FFLY_WINDOW) $(EXTRA_DEFINES)
src/maths/sq.o: src/maths/sq.asm
	nasm -f elf64 -o src/maths/sq.o src/maths/sq.asm

src/ffly_graphics.o: src/ffly_graphics.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_graphics.o src/ffly_graphics.cpp

src/maths/round.o: src/maths/round.asm
	nasm -f elf64 -o src/maths/round.o src/maths/round.asm

src/maths/ceil.o: src/maths/ceil.asm
	nasm -f elf64 -o src/maths/ceil.o src/maths/ceil.asm

src/maths/floor.o: src/maths/floor.asm
	nasm -f elf64 -o src/maths/floor.o src/maths/floor.asm

src/system/vec.o: src/system/vec.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/vec.o src/system/vec.c

src/data/swp.o: src/data/swp.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/data/swp.o src/data/swp.c

src/data/mem_ncpy.o: src/data/mem_ncpy.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/data/mem_ncpy.o src/data/mem_ncpy.c

src/system/time.o: src/system/time.c
	gcc -c -Wall -std=gnu11 -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/time.o src/system/time.c

src/system/io.o: src/system/io.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/io.o src/system/io.c

src/system/smem_buff.o: src/system/smem_buff.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/smem_buff.o src/system/smem_buff.c

src/system/config.o: src/system/config.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/config.o src/system/config.cpp

src/system/errno.o: src/system/errno.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/errno.o src/system/errno.c

src/skel_creator.o: src/skel_creator.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/skel_creator.o src/skel_creator.cpp

src/data/uni_dlen_val.o: src/data/uni_dlen_val.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/data/uni_dlen_val.o src/data/uni_dlen_val.cpp

src/graphics/draw_grid.o: src/graphics/draw_grid.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_grid.o src/graphics/draw_grid.cpp

src/ffly_memory.o: src/ffly_memory.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_memory.o src/ffly_memory.cpp

src/system/mem_tracker.o: src/system/mem_tracker.c
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/mem_tracker.o src/system/mem_tracker.c

src/ffly_audio.o: src/ffly_audio.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_audio.o src/ffly_audio.cpp

src/alsa_audio.o: src/alsa_audio.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/alsa_audio.o src/alsa_audio.cpp

src/entity_manager.o: src/entity_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/entity_manager.o src/entity_manager.cpp

src/ui/camera.o: src/ui/camera.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ui/camera.o src/ui/camera.cpp

src/graphics/crop_pixmap.o: src/graphics/crop_pixmap.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/crop_pixmap.o src/graphics/crop_pixmap.cu

src/firefly.o: src/firefly.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/firefly.o src/firefly.cpp

src/uni_manager.o: src/uni_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/uni_manager.o src/uni_manager.cpp

src/chunk_manager.o: src/chunk_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/chunk_manager.o src/chunk_manager.cpp

src/chunk_keeper.o: src/chunk_keeper.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/chunk_keeper.o src/chunk_keeper.cpp

src/system/task_handle.o: src/system/task_handle.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/task_handle.o src/system/task_handle.cpp

src/system/task_worker.o: src/system/task_worker.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/task_worker.o src/system/task_worker.cpp

src/graphics/x11_window.o: src/graphics/x11_window.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/x11_window.o src/graphics/x11_window.cpp

src/graphics/png_loader.o: src/graphics/png_loader.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/png_loader.o src/graphics/png_loader.cpp

src/networking/tcp_server.o: src/networking/tcp_server.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/tcp_server.o src/networking/tcp_server.cpp

src/networking/tcp_client.o: src/networking/tcp_client.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/tcp_client.o src/networking/tcp_client.cpp

src/networking/udp_server.o: src/networking/udp_server.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/udp_server.o src/networking/udp_server.cpp

src/networking/udp_client.o: src/networking/udp_client.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/networking/udp_client.o src/networking/udp_client.cpp

src/ffly_worker.o: src/uni_worker.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_worker.o src/uni_worker.cpp

src/ffly_server.o: src/ffly_server.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_server.o src/ffly_server.cpp

src/ffly_client.o: src/ffly_client.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_client.o src/ffly_client.cpp

src/cuda_helper.o: src/cuda_helper.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/cuda_helper.o src/cuda_helper.cpp

src/graphics/draw_rect.o: src/graphics/draw_rect.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_rect.o src/graphics/draw_rect.cu

src/graphics/draw_skelmap.o: src/graphics/draw_skelmap.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_skelmap.o src/graphics/draw_skelmap.cu

src/graphics/skelmap_loader.o: src/graphics/skelmap_loader.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/skelmap_loader.o src/graphics/skelmap_loader.cpp

src/asset_manager.o: src/asset_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/asset_manager.o src/asset_manager.cpp

src/graphics/draw_pixmap.o: src/graphics/draw_pixmap.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_pixmap.o src/graphics/draw_pixmap.cu

src/graphics/fill_pixmap.o: src/graphics/fill_pixmap.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/fill_pixmap.o src/graphics/fill_pixmap.cu

src/tests/layering.o: src/tests/layering.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/tests/layering.o src/tests/layering.cpp

src/maths/rotate_point.o: src/maths/rotate_point.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/maths/rotate_point.o src/maths/rotate_point.cpp

src/graphics/scale_pixmap.o: src/graphics/scale_pixmap.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/scale_pixmap.o src/graphics/scale_pixmap.cu

src/opencl_helper.o: src/opencl_helper.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/opencl_helper.o src/opencl_helper.cpp

src/worker_manager.o: src/worker_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/worker_manager.o src/worker_manager.cpp

src/memory/alloc_pixmap.o:src/memory/alloc_pixmap.cpp
	gcc -c -Wall -std=$(C_VERSION) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/alloc_pixmap.o src/memory/alloc_pixmap.cpp

src/graphics/draw_pixmap.clo: src/graphics/draw_pixmap.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_pixmap.clo src/graphics/draw_pixmap.cpp

src/memory/mem_alloc.o: src/memory/mem_alloc.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_alloc.o src/memory/mem_alloc.cpp

src/memory/mem_alloc.co: src/memory/mem_alloc.c
	gcc -c -Wall -std=$(C_VERSION) $(C_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_alloc.co src/memory/mem_alloc.c

#src/memory/mem_free.o: src/memory/mem_free.cpp
#	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_free.o src/memory/mem_free.cpp

src/memory/mem_free.co: src/memory/mem_free.c
	gcc -c -Wall -std=$(C_VERSION) $(C_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_free.co src/memory/mem_free.c

src/memory/mem_realloc.co: src/memory/mem_realloc.c
	gcc -c -Wall -std=$(C_VERSION) $(C_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/memory/mem_realloc.co src/memory/mem_realloc.c

src/graphics/window.o: src/graphics/window.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/window.o src/graphics/window.cpp

src/layer_manager.o: src/layer_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/layer_manager.o src/layer_manager.cpp

src/obj_manager.o: src/obj_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/obj_manager.o src/obj_manager.cpp

src/maths/cal_dist.o: src/maths/cal_dist.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/maths/cal_dist.o src/maths/cal_dist.cpp

src/gravy_manager.o: src/gravy_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/gravy_manager.o src/gravy_manager.cpp

src/ffly_studio.o: src/ffly_studio.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_studio.o src/ffly_studio.cpp

src/ffly_test.o: src/ffly_test.o
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/ffly_test.o src/ffly_test.cpp

src/gui/btn_manager.o: src/gui/btn_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/gui/btn_manager.o src/gui/btn_manager.cpp

src/graphics/draw_bitmap.o: src/graphics/draw_bitmap.cu
	nvcc -c -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/draw_bitmap.o src/graphics/draw_bitmap.cu

src/pulse_audio.o: src/pulse_audio.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/pulse_audio.o src/pulse_audio.cpp

src/graphics/xcb_window.o: src/graphics/xcb_window.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/graphics/xcb_window.o src/graphics/xcb_window.cpp

src/room_manager.o: src/room_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/room_manager.o src/room_manager.cpp

src/system/time_stamp.o: src/system/time_stamp.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/time_stamp.o src/system/time_stamp.cpp

src/gui/wd_frame.o: src/gui/wd_frame.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/gui/wd_frame.o src/gui/wd_frame.cpp

src/gui/window.o: src/gui/window.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/gui/window.o src/gui/window.cpp

src/system/event.o: src/system/event.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/system/event.o src/system/event.cpp

src/data/scale_pixmap.o: src/data/scale_pixmap.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/data/scale_pixmap.o src/data/scale_pixmap.cpp

src/font.o: src/font.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/font.o src/font.cpp

src/player_manager.o: src/player_manager.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/player_manager.o src/player_manager.cpp

src/player_handler.o: src/player_handler.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/player_handler.o src/player_handler.cpp

src/worker_handler.o: src/worker_handler.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/worker_handler.o src/worker_handler.cpp

src/uni_worker.o: src/uni_worker.cpp
	g++ -c -Wall -std=$(CXX_VERSION) $(CXX_IFLAGS) -D$(FFLY_TARGET) $(FFLY_DEFINES) -o src/uni_worker.o src/uni_worker.cpp

relocate_headers:
	if ! [ -d $(CURR_DIR)/inc/firefly ]; then \
		mkdir $(CURR_DIR)/inc/firefly; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/graphics ]; then \
		mkdir $(CURR_DIR)/inc/firefly/graphics; \
	fi

	cp $(CURR_DIR)/src/graphics/*.hpp $(CURR_DIR)/inc/firefly/graphics
	cp $(CURR_DIR)/src/graphics/*.h $(CURR_DIR)/inc/firefly/graphics

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

	if ! [ -d $(CURR_DIR)/inc/firefly/ui ]; then \
		mkdir $(CURR_DIR)/inc/firefly/ui; \
	fi

	if ! [ -d $(CURR_DIR)/inc/firefly/data ]; then \
		mkdir $(CURR_DIR)/inc/firefly/data; \
	fi

	cp $(CURR_DIR)/src/data/*.hpp $(CURR_DIR)/inc/firefly/data 2>/dev/null
	cp $(CURR_DIR)/src/data/*.h $(CURR_DIR)/inc/firefly/data 2>/dev/null

	cp $(CURR_DIR)/src/ui/*.hpp $(CURR_DIR)/inc/firefly/ui

	#cp $(CURR_DIR)/src/gui/*.h $(CURR_DIR)/inc/firefly/gui
	cp $(CURR_DIR)/src/gui/*.hpp $(CURR_DIR)/inc/firefly/gui

	cp $(CURR_DIR)/src/system/*.h $(CURR_DIR)/inc/firefly/system
	cp $(CURR_DIR)/src/system/*.hpp $(CURR_DIR)/inc/firefly/system

	cp $(CURR_DIR)/src/memory/*.h $(CURR_DIR)/inc/firefly/memory
	cp $(CURR_DIR)/src/memory/*.hpp $(CURR_DIR)/inc/firefly/memory

	cp $(CURR_DIR)/src/maths/*.h $(CURR_DIR)/inc/firefly/maths
	cp $(CURR_DIR)/src/maths/*.hpp $(CURR_DIR)/inc/firefly/maths

	cp $(CURR_DIR)/src/tests/*.hpp $(CURR_DIR)/inc/firefly/tests

	cp $(CURR_DIR)/src/types/*.h $(CURR_DIR)/inc/firefly/types 2>/dev/null
	cp $(CURR_DIR)/src/types/*.hpp $(CURR_DIR)/inc/firefly/types 2>/dev/null

	cp $(CURR_DIR)/src/*.h $(CURR_DIR)/inc/firefly
	cp $(CURR_DIR)/src/*.hpp $(CURR_DIR)/inc/firefly

	cp $(CURR_DIR)/mdlint/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/getdigit/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/intlen/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/to_string/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/strcmb/inc/*.hpp $(CURR_DIR)/inc
	cp $(CURR_DIR)/serializer/inc/*.hpp $(CURR_DIR)/inc

# core libraries that are needed by design
libraries:
	cd termio; make; cd ../;
	cd intlen; make ARC64 MDLINT_INC=$(CURR_DIR)/mdlint/inc; cd ../;
	cd nibbles; make MDLINT_INC=$(CURR_DIR)/mdlint/inc; cd ../;
	cd getdigit; make ARC64 MDLINT_INC=$(CURR_DIR)/mdlint/inc INTLEN_INC=$(CURR_DIR)/intlen/inc INTLEN_LIB=$(CURR_DIR)/intlen/lib; cd ../;
	cd to_string; make ECHAR_T=$(CURR_DIR)/echar_t/inc MDLINT_INC=$(CURR_DIR)/mdlint/inc GETDIGIT_INC=$(CURR_DIR)/getdigit/inc INTLEN_INC=$(CURR_DIR)/intlen/inc GETDIGIT_LIB=$(CURR_DIR)/getdigit/lib INTLEN_LIB=$(CURR_DIR)/intlen/lib; cd ../;
	cd strcmb; make MDLINT_INC=$(CURR_DIR)/echar_t/inc MDLINT_INC=$(CURR_DIR)/mdlint/inc ARC=$(ARC); cd ../;
	cd tagged_memory; make LIB_PATH=$(CURR_DIR)/; cd ../;
	cd emu2d; make ARC=$(ARC) MDLINT_INC=$(CURR_DIR)/mdlint/inc; cd ../;
	cd emu3d; make ARC=$(ARC) MDLINT_INC=$(CURR_DIR)/mdlint/inc; cd ../;

ffly_worker: $(FFLY_OBJS)
	ld -r -o lib/ffly_worker.o $(FFLY_OBJS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_worker.a lib/ffly_worker.o
#	make relocate_headers

ffly_server: $(FFLY_OBJS)
	ld -r -o lib/ffly_server.o $(FFLY_OBJS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_server.a lib/ffly_server.o
#	make relocate_headers

ffly_client: $(FFLY_OBJS)
	ld -r -o lib/ffly_client.o $(FFLY_OBJS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_client.a lib/ffly_client.o
#	make relocate_headers

ffly_studio: $(FFLY_OBJS) src/ffly_studio.o
	ld -r -o lib/ffly_studio.o $(FFLY_OBJS) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_studio.a lib/ffly_studio.o
#	make relocate_headers

ffly_test: $(FFLY_OBJS) src/ffly_test.o
	ld -r -o lib/ffly_test.o $(FFLY_OBJS)
	ar rcs lib/libffly_test.a lib/ffly_test.o
#	make relocate_headers
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
	cd nibbles; make clean; cd ../;
	cd termio; make clean; cd ../;
	cd intlen; make clean; cd ../;
	cd getdigit; make clean; cd ../;
	cd to_string; make clean; cd ../;
	cd strcmb; make clean; cd ../;
	cd tagged_memory; make clean; cd ../;

	rm -f src/ui/*.o src/gui/*.o src/memory/*.o src/memory/*.co src/graphics/*.o src/graphics/*.clo src/networking/*.o src/maths/*.o src/tests/*.o src/system/*.o src/*.o src/data/*.o *.exec #bin/*.exec
	rm -rf $(CURR_DIR)/inc/* $(CURR_DIR)/lib/*
