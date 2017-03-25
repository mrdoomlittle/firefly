all:
	cd ../; \
	make src/memory/alloc_pixmap.o; \
	make src/memory/mem_alloc.o; \
	make src/memory/mem_free.o; \
	cd src;
	g++ -c -std=c++11 uni_manager.cpp
	g++ -c -std=c++11 chunk_keeper.cpp
	g++ -c -std=c++11 chunk_manager.cpp
	g++ -std=c++11 main.cpp chunk_keeper.o uni_manager.o chunk_manager.o memory/alloc_pixmap.o memory/mem_alloc.o memory/mem_free.o
