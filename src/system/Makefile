DEFINES=-D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
all:
#	g++ -c -std=c++11 ../memory/mem_alloc.cpp
#//	g++ -c -std=c++11 ../memory/mem_free.cpp
#//	g++ -c -std=c++11 ../memory/mem_realloc.cpp
	gcc -c -std=c11 $(DEFINES) errno.c
	gcc -c -std=c11 $(DEFINES) ../memory/mem_free.c
	gcc -c -std=c11 $(DEFINES) ../memory/mem_alloc.c
	gcc -c -std=c11 $(DEFINES) ../memory/mem_realloc.c
	g++ -c -std=c++11 $(DEFINES) -o aa.o ../memory/mem_alloc.cpp
	g++ -c -std=c++11 $(DEFINES) -o bb.o ../memory/mem_free.cpp
	g++ -c -std=c++11 $(DEFINES) -o cc.o ../memory/mem_realloc.cpp
	gcc -c -std=c11 $(DEFINES) -o mem_allocr.o mem_allocr.c
	gcc -c -std=c11 $(DEFINES) -o mem_tracker.o mem_tracker.c
	gcc -c -std=c11 $(DEFINES) -o smem_buff.o smem_buff.c
	g++ -std=c++11 main.cpp $(DEFINES) errno.o mem_alloc.o mem_free.o cc.o mem_tracker.o smem_buff.o aa.o bb.o mem_allocr.o mem_realloc.o
