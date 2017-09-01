DEFINES=-I../eint_t/inc -D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
all:
	rm -f *.o
	gcc -c -std=c11 -DARC64 $(DEFINES) system/mem_tracker.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/alloc_pixmap.c
	gcc -c -std=c11 -DARC64 $(DEFINES) data/mem_ncpy.c
	gcc -c -std=c11 -DARC64 $(DEFINES) data/swp.c
	gcc -c -std=c11 -DARC64 $(DEFINES) system/io.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/mem_alloc.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/mem_free.c
	gcc -c -std=c11 -DARC64 $(DEFINES) memory/mem_realloc.c
	gcc -c -std=c11 -DARC64 $(DEFINES) -o chunk_keeper.o chunk_keeper.c
	g++ -c -std=c++11 -DARC64 $(DEFINES) -o chunk_manager.o chunk_manager.cpp
	gcc -c -std=c11 -DARC64 $(DEFINES) data/mem_cmp.c
	gcc -c -std=c11 -DARC64 $(DEFINES) data/find.c
	g++ -std=c++11 -DARC64 $(DEFINES) -o main main.cpp find.o mem_tracker.o chunk_manager.o mem_cmp.o swp.o alloc_pixmap.o mem_ncpy.o io.o chunk_keeper.o mem_alloc.o mem_free.o mem_realloc.o
#all

##	g++ -DARC64 -I../serializer/inc -L../serializer/lib -std=c++11 player_manager.cpp

#all:
#	cd ../; \
#	make src/memory/alloc_pixmap.o ARC=-DARC64; \
#	make src/memory/mem_alloc.o ARC=-DARC64; \
#	make src/memory/mem_free.o ARC=-DARC64; \
#	cd src;
#	g++ -c -DARC64 -std=c++11 uni_manager.cpp
#	g++ -c -DARC64 -std=c++11 chunk_keeper.cpp
#	g++ -c -DARC64 -std=c++11 chunk_manager.cpp
#	g++ -c -DARC64 -std=c++11 chunk_handler.cpp
#	g++ -std=c++11 -DARC64 main.cpp chunk_keeper.o chunk_handler.o uni_manager.o chunk_manager.o memory/alloc_pixmap.o memory/mem_alloc.o memory/mem_free.o -lpthread
