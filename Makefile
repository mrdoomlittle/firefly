CXXFLAGS=
LDFLAGS=
LLFLAGS=

sandbox:
	g++ -c -std=c++11 -DARC64 -o src/x11_window.o src/x11_window.cpp
#	g++ -std=c++11 -DARC64 -o sandbox.exec sandbox.cpp src/x11_window.o -lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut

	g++ -c -std=c++11 -DARC64 -o src/udp_server.o src/udp_server.cpp
	g++ -c -std=c++11 -DARC64 -o src/udp_client.o src/udp_client.cpp

	g++ -std=c++11 -DARC64 -o sandbox.exec sandbox.cpp src/x11_window.o src/udp_client.o -lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut
	g++ -std=c++11 -DARC64 -o server.exec server.cpp src/udp_server.o
clean:
	rm -f src/*.o
	rm -f *.exec

#all:

#client: ffly_client
#server: ffly_server

#ffly_client: src/ffly_client.o
#	g++ -std=c++11 -L/usr/local/lib -DARC64 -o ffly_client.exec src/ffly_client.o \
#	-lboost_system -lboost_filesystem -lpthread -lboost_thread -lX11 -lGL -lGLU -lglut
#
#clean:
#	rm -f src/*.o
#	rm -f *.exec
#
#src/ffly_client.o: src/ffly_client.cpp
#	g++ -c -std=c++11 -DARC64 -o src/ffly_client.o src/ffly_client.cpp
