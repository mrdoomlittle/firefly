#SHELL := /bin/bash
ifeq ($(ffly_target), ffly_server)
 override ffly_objs+= src/ffly_server.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/graphics/png_loader.o \
 src/worker_manager.o src/memory/alloc_pixelmap.o src/graphics/draw_pixmap.clo src/player_manager.o src/player_handler.o src/worker_handler.o
else ifeq ($(ffly_target), ffly_client)
 override ffly_objs+= src/ffly_client.o src/graphics/window.o src/audio/alsa.o src/audio/pulse.o src/ffly_audio.o src/asset_manager.o src/graphics/png_loader.o src/system/time_stamp.o src/system/event.o \
 src/layer_manager.o src/room_manager.o
else ifeq ($(ffly_target), ffly_studio)
 override ffly_objs+= src/skel_creator.o src/graphics/draw_grid.o src/ffly_audio.o src/memory/alloc_pixelmap.o src/graphics/window.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
 src/gui/btn_manager.o src/graphics/draw_skelmap.o src/graphics/draw_bitmap.o src/pulse_audio.o src/maths/rotate_point.o \
 src/graphics/png_loader.o src/room_manager.o src/asset_manager.o src/system/time_stamp.o  src/graphics/draw_rect.o \
 src/gui/wd_frame.o src/gui/window.o src/data/scale_pixmap.o src/graphics/draw_pixmap.clo src/system/task_handle.o src/system/task_worker.o \
 src/system/smem_buff.o src/system/event.o
else ifeq ($(ffly_target), ffly_worker)
 override ffly_objs+= src/uni_worker.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/png_loader.o src/memory/alloc_pixelmap.o
else ifeq ($(ffly_target), ffly_test)
 override ffly_objs+= src/graphics/window.o src/audio/alsa.o src/audio/pulse.o src/ffly_audio.o src/asset_manager.o src/graphics/png_loader.o src/system/time_stamp.o
else ifeq ($(ffly_target), ffly_bare)
 override ffly_objs+= 
endif

#ifeq ($(GPU_CL_TYPE), -DUSING_OPENCL)
# CXXFLAGS+= -L/usr/local/lib/x86_64/sdk
# ffly_objs+= src/opencl_helper.o
#else ifeq ($(GPU_CL_TYPE), -DUSING_CUDA)
# ffly_objs+= src/cuda_helper.o
#endif

#ffly_objs+= src/firefly.o
# lower level core - things that are needed for things to work
# maths
override ffly_objs+= src/maths/round.o src/maths/ceil.o src/maths/floor.o src/maths/sq.o src/maths/is_inside.o
# data
override ffly_objs+= src/data/swp.o src/data/mem_cpy.o src/data/str_len.o src/data/mem_dupe.o src/data/mem_set.o src/data/str_dupe.o src/data/mem_cmp.o src/data/str_cmp.o
# system
override ffly_objs+= src/system/arr.o src/system/buff.o src/system/vec.o src/system/time.o src/system/config.o src/system/errno.o src/system/io.o src/system/thread.o src/system/flags.o \
src/system/mutex.o src/system/atomic_op.o src/system/queue.o src/system/util/hash.o src/system/map.o src/system/file.o src/system/dir.o src/system/task_pool.o \
src/system/task_worker.o src/system/sys_nanosleep.o src/system/mem_blk.o src/system/cond_lock.o src/system/signal.o
# graphics
override ffly_objs+= src/graphics/job.o src/graphics/pipe.o src/graphics/fill.o src/graphics/copy.o src/graphics/draw.o #src/graphics/draw_pixelmap.o src/graphics/fill_pixmap.o src/graphics/fill_pixelmap.o
#ffly_objs+= src/ffly_graphics.o
# memory
override ffly_objs+= src/ffly_memory.o src/memory/mem_alloc.o src/memory/mem_free.o src/memory/mem_realloc.o src/memory/alloc_pixelmap.o


override ffly_objs+= src/act.o
ifeq ($(shell bash find.bash "$(ffly_flags)" "--mem-agent"), 0)
	override ffly_objs+= src/system/mal_track.o
endif

ifneq ($(ffly_target), $(filter $(ffly_target), ffly_server ffly_worker))
 ifeq ($(ffly_window), x11)
  override ffly_objs+= src/graphics/x11_wd.o
 else ifeq ($(ffly_window), xcb)
  override ffly_objs+= src/graphics/xcb_wd.o
 endif
endif

ifeq ($(ffly_target), ffly_worker)
all: ffly_worker
else ifeq ($(ffly_target), ffly_server)
all: ffly_server
else ifeq ($(ffly_target), ffly_client)
all: ffly_client
else ifeq ($(ffly_target), ffly_studio)
all: ffly_studio
else ifeq ($(ffly_target), ffly_test)
all: ffly_test
else ifeq ($(ffly_target), ffly_bare)
all ffly_bare
endif

override ffly_objs+= src/firefly.o
override ffly_defines+= -D__USING_PULSE_AUDIO
src/graphics/draw.o: src/graphics/draw.cu src/graphics/draw.cpp
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw.o.1 src/graphics/draw.cu
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw.o.2 src/graphics/draw.cpp
	ld -r src/graphics/draw.o.1 src/graphics/draw.o.2 -o src/graphics/draw.o

src/system/arr.o: src/system/arr.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/arr.o src/system/arr.c

src/graphics/job.o: src/graphics/job.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/graphics/job.o src/graphics/job.c

src/graphics/pipe.o: src/graphics/pipe.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/graphics/pipe.o src/graphics/pipe.c

src/act.o: src/act.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/act.o src/act.c

src/system/mem_blk.o: src/system/mem_blk.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/mem_blk.o src/system/mem_blk.c

src/system/task_pool.o: src/system/task_pool.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/task_pool.o src/system/task_pool.c

src/system/task_worker.o: src/system/task_worker.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/task_worker.o src/system/task_worker.c

src/system/file.o: src/system/file.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/file.o src/system/file.c

src/system/dir.o: src/system/dir.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/dir.o src/system/dir.c

src/chunk_keeper.o:src/chunk_keeper.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/chunk_keeper.o src/chunk_keeper.c

src/data/str_cmp.o: src/data/str_cmp.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/str_cmp.o src/data/str_cmp.c

src/data/mem_cmp.o: src/data/mem_cmp.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/mem_cmp.o src/data/mem_cmp.c

src/data/str_dupe.o: src/data/str_dupe.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/str_dupe.o src/data/str_dupe.c

src/system/util/hash.o: src/system/util/hash.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/util/hash.o src/system/util/hash.c

src/system/map.o: src/system/map.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/map.o src/system/map.c

src/audio/alsa.o: src/audio/alsa.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/audio/alsa.o src/audio/alsa.c

src/audio/pulse.o: src/audio/pulse.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/audio/pulse.o src/audio/pulse.c

src/system/flags.o: src/system/flags.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/flags.o src/system/flags.c

src/system/thread.o: src/system/thread.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=gnu11 -D__$(ffly_target) $(ffly_defines) -o src/system/thread.o src/system/thread.c

src/maths/is_inside.o: src/maths/is_inside.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/maths/is_inside.o src/maths/is_inside.c

src/system/signal.o: src/system/asm/signal.asm
	nasm -f elf64 -o src/system/signal.o src/system/asm/signal.asm

src/system/cond_lock.o: src/system/asm/cond_lock.asm
	nasm -f elf64 -o src/system/cond_lock.o src/system/asm/cond_lock.asm

src/maths/sq.o: src/maths/asm/sq.asm
	nasm -f elf64 -o src/maths/sq.o src/maths/asm/sq.asm

src/ffly_graphics.o: src/ffly_graphics.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_graphics.o src/ffly_graphics.cpp

src/system/atomic_op.o: src/system/asm/atomic_op.asm
	nasm -f elf64 -o src/system/atomic_op.o src/system/asm/atomic_op.asm

src/system/mutex.o: src/system/asm/mutex.asm
	nasm -f elf64 -o src/system/mutex.o src/system/asm/mutex.asm

src/system/sys_nanosleep.o: src/system/asm/sys_nanosleep.asm
	nasm -f elf64 -o src/system/sys_nanosleep.o src/system/asm/sys_nanosleep.asm

src/maths/round.o: src/maths/asm/round.asm
	nasm -f elf64 -o src/maths/round.o src/maths/asm/round.asm

src/maths/ceil.o: src/maths/asm/ceil.asm
	nasm -f elf64 -o src/maths/ceil.o src/maths/asm/ceil.asm

src/maths/floor.o: src/maths/asm/floor.asm
	nasm -f elf64 -o src/maths/floor.o src/maths/asm/floor.asm

src/system/buff.o: src/system/buff.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/buff.o src/system/buff.c

src/system/vec.o: src/system/vec.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/vec.o src/system/vec.c

src/data/swp.o: src/data/swp.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/swp.o src/data/swp.c

src/data/mem_set.o: src/data/mem_set.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/mem_set.o src/data/mem_set.c

src/data/mem_cpy.o: src/data/mem_cpy.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/mem_cpy.o src/data/mem_cpy.c

src/data/str_len.o: src/data/str_len.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/str_len.o src/data/str_len.c

src/data/mem_dupe.o: src/data/mem_dupe.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/data/mem_dupe.o src/data/mem_dupe.c

src/system/time.o: src/system/time.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=gnu11 -D__$(ffly_target) $(ffly_defines) -o src/system/time.o src/system/time.c

src/system/io.o: src/system/io.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/io.o src/system/io.c

src/system/smem_buff.o: src/system/smem_buff.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/smem_buff.o src/system/smem_buff.c

src/system/config.o: src/system/config.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/system/config.o src/system/config.cpp

src/system/errno.o: src/system/errno.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/errno.o src/system/errno.c

src/skel_creator.o: src/skel_creator.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/skel_creator.o src/skel_creator.cpp

src/data/uni_dlen_val.o: src/data/uni_dlen_val.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/data/uni_dlen_val.o src/data/uni_dlen_val.cpp

src/graphics/draw_grid.o: src/graphics/draw_grid.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw_grid.o src/graphics/draw_grid.cpp

src/ffly_memory.o: src/ffly_memory.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_memory.o src/ffly_memory.cpp

src/system/mal_track.o: src/system/mal_track.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/system/mal_track.o src/system/mal_track.c

src/ffly_audio.o: src/ffly_audio.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/ffly_audio.o src/ffly_audio.c

src/alsa_audio.o: src/alsa_audio.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/alsa_audio.o src/alsa_audio.cpp

src/entity_manager.o: src/entity_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/entity_manager.o src/entity_manager.cpp

src/ui/camera.o: src/ui/camera.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ui/camera.o src/ui/camera.cpp

src/graphics/crop_pixmap.o: src/graphics/crop_pixmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/crop_pixmap.o src/graphics/crop_pixmap.cu

src/firefly.o: src/firefly.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/firefly.o src/firefly.cpp

src/uni_manager.o: src/uni_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/uni_manager.o src/uni_manager.cpp

src/chunk_manager.o: src/chunk_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/chunk_manager.o src/chunk_manager.cpp

#src/system/task_handle.o: src/system/task_handle.cpp
#	$(ffly_cxx) -c -Wall -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/system/task_handle.o src/system/task_handle.cpp

#src/system/task_worker.o: src/system/task_worker.cpp
#	$(ffly_cxx) -c -Wall -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/system/task_worker.o src/system/task_worker.cpp

src/graphics/x11_wd.o: src/graphics/x11_wd.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_cflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/graphics/x11_wd.o src/graphics/x11_wd.c

src/graphics/png_loader.o: src/graphics/png_loader.c
	 $(ffly_cc) -c $(ffly_cflags) $(ffly_cflags) -std=gnu11 -D__$(ffly_target) $(ffly_defines) -o src/graphics/png_loader.o src/graphics/png_loader.c

src/networking/tcp_server.o: src/networking/tcp_server.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/networking/tcp_server.o src/networking/tcp_server.cpp

src/networking/tcp_client.o: src/networking/tcp_client.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/networking/tcp_client.o src/networking/tcp_client.cpp

src/networking/udp_server.o: src/networking/udp_server.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/networking/udp_server.o src/networking/udp_server.cpp

src/networking/udp_client.o: src/networking/udp_client.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/networking/udp_client.o src/networking/udp_client.cpp

src/ffly_worker.o: src/uni_worker.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_worker.o src/uni_worker.cpp

src/ffly_server.o: src/ffly_server.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_server.o src/ffly_server.cpp

src/ffly_client.o: src/ffly_client.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_client.o src/ffly_client.cpp

src/cuda_helper.o: src/cuda_helper.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/cuda_helper.o src/cuda_helper.cpp

src/graphics/draw_rect.o: src/graphics/draw_rect.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw_rect.o src/graphics/draw_rect.cu

src/graphics/draw_skelmap.o: src/graphics/draw_skelmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw_skelmap.o src/graphics/draw_skelmap.cu

src/graphics/skelmap_loader.o: src/graphics/skelmap_loader.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/skelmap_loader.o src/graphics/skelmap_loader.cpp

src/asset_manager.o: src/asset_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/asset_manager.o src/asset_manager.cpp

src/graphics/draw_pixelmap.o: src/graphics/draw_pixelmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw_pixelmap.o src/graphics/draw_pixelmap.cu

src/graphics/fill.o: src/graphics/fill.cu src/graphics/fill.cpp
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/fill.o.1 src/graphics/fill.cu
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/fill.o.2 src/graphics/fill.cpp
	ld -r src/graphics/fill.o.1 src/graphics/fill.o.2 -o src/graphics/fill.o

src/graphics/copy.o: src/graphics/copy.cu src/graphics/copy.cpp
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/copy.o.1 src/graphics/copy.cu
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/copy.o.2 src/graphics/copy.cpp
	ld -r src/graphics/copy.o.1 src/graphics/copy.o.2 -o src/graphics/copy.o

src/graphics/fill_pixelmap.o: src/graphics/fill_pixelmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/fill_pixelmap.o src/graphics/fill_pixelmap.cu

src/graphics/fill_pixmap.o: src/graphics/fill_pixmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/fill_pixmap.o src/graphics/fill_pixmap.cu

src/tests/layering.o: src/tests/layering.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/tests/layering.o src/tests/layering.cpp

src/maths/rotate_point.o: src/maths/rotate_point.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/maths/rotate_point.o src/maths/rotate_point.cpp

src/graphics/scale_pixmap.o: src/graphics/scale_pixmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/scale_pixmap.o src/graphics/scale_pixmap.cu

src/opencl_helper.o: src/opencl_helper.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/opencl_helper.o src/opencl_helper.cpp

src/worker_manager.o: src/worker_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/worker_manager.o src/worker_manager.cpp

src/memory/alloc_pixelmap.o: src/memory/alloc_pixelmap.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/memory/alloc_pixelmap.o src/memory/alloc_pixelmap.c

src/graphics/draw_pixmap.clo: src/graphics/draw_pixmap.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw_pixmap.clo src/graphics/draw_pixmap.cpp

src/memory/mem_alloc.o: src/memory/mem_alloc.cpp src/memory/mem_alloc.c
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_alloc.o.1 src/memory/mem_alloc.cpp
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) $(C_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_alloc.o.2 src/memory/mem_alloc.c
	ld -r src/memory/mem_alloc.o.1 src/memory/mem_alloc.o.2 -o src/memory/mem_alloc.o

#src/memory/mem_alloc.co: src/memory/mem_alloc.c
#	$(ffly_cc) -c -Wall -std=$(ffly_stdc) $(C_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_alloc.co src/memory/mem_alloc.c

#src/memory/mem_free.o: src/memory/mem_free.cpp
#	$(ffly_cxx) -c -Wall -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_free.o src/memory/mem_free.cpp

src/memory/mem_free.o: src/memory/mem_free.cpp src/memory/mem_free.c
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_free.o.1 src/memory/mem_free.cpp
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) $(C_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_free.o.2 src/memory/mem_free.c
	ld -r src/memory/mem_free.o.1 src/memory/mem_free.o.2 -o src/memory/mem_free.o

src/memory/mem_realloc.o: src/memory/mem_realloc.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) $(C_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/memory/mem_realloc.o src/memory/mem_realloc.c

src/graphics/window.o: src/graphics/window.cpp src/graphics/window.c
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/window.o.1 src/graphics/window.cpp
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) $(C_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/window.o.2 src/graphics/window.c
	ld -r src/graphics/window.o.1 src/graphics/window.o.2 -o src/graphics/window.o

src/layer_manager.o: src/layer_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/layer_manager.o src/layer_manager.cpp

src/obj_manager.o: src/obj_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/obj_manager.o src/obj_manager.cpp

src/maths/cal_dist.o: src/maths/cal_dist.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/maths/cal_dist.o src/maths/cal_dist.cpp

src/gravy_manager.o: src/gravy_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/gravy_manager.o src/gravy_manager.cpp

src/ffly_studio.o: src/ffly_studio.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_studio.o src/ffly_studio.cpp

src/ffly_test.o: src/ffly_test.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/ffly_test.o src/ffly_test.cpp

src/gui/btn_manager.o: src/gui/btn_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/gui/btn_manager.o src/gui/btn_manager.cpp

src/graphics/draw_bitmap.o: src/graphics/draw_bitmap.cu
	$(ffly_nvcc) -c -std=c++11 $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/graphics/draw_bitmap.o src/graphics/draw_bitmap.cu

src/pulse_audio.o: src/pulse_audio.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/pulse_audio.o src/pulse_audio.cpp

src/graphics/xcb_wd.o: src/graphics/xcb_wd.c
	$(ffly_cc) -c $(ffly_cflags) $(ffly_ccflags) -std=$(ffly_stdc) -D__$(ffly_target) $(ffly_defines) -o src/graphics/xcb_wd.o src/graphics/xcb_wd.c

src/room_manager.o: src/room_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/room_manager.o src/room_manager.cpp

src/system/time_stamp.o: src/system/time_stamp.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/system/time_stamp.o src/system/time_stamp.cpp

src/gui/wd_frame.o: src/gui/wd_frame.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/gui/wd_frame.o src/gui/wd_frame.cpp

src/gui/window.o: src/gui/window.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/gui/window.o src/gui/window.cpp

src/system/event.o: src/system/event.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/system/event.o src/system/event.cpp

src/data/scale_pixmap.o: src/data/scale_pixmap.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/data/scale_pixmap.o src/data/scale_pixmap.cpp

src/font.o: src/font.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/font.o src/font.cpp

src/player_manager.o: src/player_manager.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/player_manager.o src/player_manager.cpp

src/player_handler.o: src/player_handler.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/player_handler.o src/player_handler.cpp

src/worker_handler.o: src/worker_handler.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/worker_handler.o src/worker_handler.cpp

src/uni_worker.o: src/uni_worker.cpp
	$(ffly_cxx) -c $(ffly_cflags) $(ffly_cxxflags) -std=$(ffly_stdcxx) $(CXX_IFLAGS) -D__$(ffly_target) $(ffly_defines) -o src/uni_worker.o src/uni_worker.cpp

ffly_worker: $(ffly_objs)
	ld -r -o lib/ffly_worker.o $(ffly_objs) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_worker.a lib/ffly_worker.o
#	make relocate_headers

ffly_server: $(ffly_objs)
	ld -r -o lib/ffly_server.o $(ffly_objs) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_server.a lib/ffly_server.o
#	make relocate_headers

ffly_client: $(ffly_objs)
	ld -r -o lib/ffly_client.o $(ffly_objs) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_client.a lib/ffly_client.o
#	make relocate_headers

ffly_studio: $(ffly_objs) src/ffly_studio.o
	ld -r -o lib/ffly_studio.o $(ffly_objs) #$(LIBRARY_OBJS)
	ar rcs lib/libffly_studio.a lib/ffly_studio.o
#	make relocate_headers

ffly_test: $(ffly_objs) src/ffly_test.o
	ld -r -o lib/ffly_test.o $(ffly_objs)
	ar rcs lib/libffly_test.a lib/ffly_test.o

ffly_bare: $(ffly_objs)
	ld -r -o lib/ffly_bare.o $(ffly_objs)
	ar rcs lib/libffly_bare.a lib/ffly_bare.o

#	make relocate_headers
	#$(ffly_cxx) -std=c++11 -Iinc -Llib -Wall -o bin/ffly_studio src/ffly_studio.o -lffly_studio

#ffly_server: required src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/opencl_helper.o
#	ld -r -o lib/ffly_server.o src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/opencl_helper.o
#
#	ar rcs lib/libffly_server.a lib/ffly_server.o
#	rm lib/ffly_server.o
#
#	make move_headers;
#	make move_libs;

#ffly_client: required src/ffly_client.o src/graphics/x11_wd.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
#	src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/tests/layering.o src/maths/rotate_point.o src/graphics/scale_pixmap.o
#	ld -r -o lib/ffly_client.o src/ffly_client.o src/graphics/x11_wd.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
#	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o \
#	src/tests/layering.o src/maths/rotate_point.o src/graphics/scale_pixmap.o
#
#	ar rcs lib/libffly_client.a lib/ffly_client.o
#	rm lib/ffly_client.o
#
#	make move_headers;
#	make move_libs;

#move_libs:#
#	cp $(ROOT_DIR)/getdigit/lib/*.a $(ROOT_DIR)/lib
#	cp $(ROOT_DIR)/intlen/lib/*.a $(ROOT_DIR)/lib
#	cp $(ROOT_DIR)/to_string/lib/*.a $(ROOT_DIR)/lib
#	cp $(ROOT_DIR)/strcmb/lib/*.a $(ROOT_DIR)/lib



#src/ffly_memory.o: src/memory/mem_alloc.o src/memory/mem_free.o src/memory/alloc_pixelmap.o
#	ld -r -o src/ffly_memory.o src/memory/mem_alloc.o src/memory/mem_free.o src/memory/alloc_pixelmap.o

#example_client: required src/ffly_client.o src/graphics/x11_wd.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o \
#	src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/ffly_memory.o
#	$(ffly_cxx) -std=c++11 $(CXXFLAGS) -DUSING_CUDA -DFFLY_CLIENT $(CUDA) -Wall $(ARC) -o bin/example_client.exec example_client.cpp src/ffly_client.o src/graphics/x11_wd.o src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
#	src/graphics/draw_rect.o src/graphics/draw_skelmap.o src/graphics/skelmap_loader.o src/asset_manager.o src/graphics/draw_pixmap.o src/graphics/fill_pixmap.o src/ffly_memory.o \
#	-lto_string -lgetdigit -lintlen -lstrcmb -lemu2d -lpulse -lpulse-simple -lpng16 -lcuda -lcudart -lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut

#example_server: src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o \
#	src/opencl_helper.o src/worker_manager.o src/memory/alloc_pixelmap.o src/graphics/draw_pixmap.clo
#	$(ffly_cxx) -std=c++11 $(CXXFLAGS) -DUSING_OPENCL -DFFLY_SERVER -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/example_server.exec example_server.cpp \
#	src/ffly_server.o src/graphics/png_loader.o src/networking/tcp_server.o src/networking/tcp_client.o src/networking/udp_server.o src/networking/udp_client.o src/opencl_helper.o src/worker_manager.o src/memory/alloc_pixelmap.o \
#	src/graphics/draw_pixmap.clo -lpng16 -lboost_system -lemu2d -lOpenCL -lboost_filesystem -lpthread -lboost_thread

example_worker:
	make ffly_target=-DFFLY_WORKER;
	$(ffly_cxx) -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -DFFLY_WORKER -Iinc -Llib -Wall -o bin/example_worker.exec src/uni_worker.cpp -lffly_worker $(LDFLAGS)

example_server:
	make ffly_target=-DFFLY_SERVER;
	$(ffly_cxx) -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -DFFLY_SERVER -Iinc -Llib -Wall -o bin/example_server.exec example_server.cpp -lffly_server $(LDFLAGS)

example_client:
	make ffly_target=-DFFLY_CLIENT;
	$(ffly_cxx) -std=c++11 $(CXXFLAGS) -DFFLY_CLIENT $(CUDART_INC) $(CUDART_LIB) -Iinc -Llib -Wall -o bin/example_client.exec example_client.cpp -lffly_client $(LDFLAGS) -lX11 -lGL -lGLU -lglut

#example_worker:
#	make ffly_target=-DFFLY_WORKER;

#uni_worker: src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o src/ffly_memory.o
#	$(ffly_cxx) -std=c++11 $(CXXFLAGS) -L/usr/local/lib/x86_64/sdk -Wall $(ARC) -o bin/uni_worker.exec src/uni_worker.cpp src/graphics/png_loader.o src/networking/tcp_client.o src/networking/udp_client.o \
#	src/ffly_memory.o -lpng16 -lboost_system -lOpenCL -lboost_filesystem -lpthread -lboost_thread

skel_creator: src/graphics/x11_wd.o
	$(ffly_nvcc) -c -std=c++11 -DARC64 $(CUDA) -o draw_grid.o draw_grid.cu
	$(ffly_cxx) -std=c++11 $(CUDA) -DARC64 -L/usr/local/lib -I/usr/local/include -o skel_creator.exec skel_creator.cpp draw_grid.o src/graphics/x11_wd.o -lcuda -lcudart -lboost_system -lpthread -lboost_thread -lemu2d -lX11 -lGL -lGLU -lglut

clean:
	sh clean.sh
