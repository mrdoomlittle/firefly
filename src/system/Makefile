DEFINES=-D__DEBUG_ENABLED -D__WITH_MEM_TRACKER
all:
#	g++ -c -std=c++11 ../memory/mem_alloc.cpp
#//	g++ -c -std=c++11 ../memory/mem_free.cpp
#//	g++ -c -std=c++11 ../memory/mem_realloc.cpp
	gcc -c -std=c11 $(DEFINES) ../memory/mem_free.c
	gcc -c -std=c11 $(DEFINES) ../memory/mem_alloc.c
	g++ -c -std=c++11 $(DEFINES) -o aa.o ../memory/mem_alloc.cpp
	g++ -c -std=c++11 $(DEFINES) -o bb.o ../memory/mem_free.cpp
	gcc -c -std=c11 $(DEFINES) ../memory/mem_realloc.c
	gcc -c -std=c11 $(DEFINES) mem_tracker.c
	g++ -c -std=c++11 $(DEFINES) ../data/conv_dsm.cpp
	g++ -std=c++11 main.cpp $(DEFINES) mem_alloc.o mem_free.o mem_realloc.o mem_tracker.o aa.o bb.o conv_dsm.o
