CXXFLAGS=
LDFLAGS=
LLFLAGS=
all:

sandbox: src/ffly_client.o
	g++ -std=c++11 -o sandbox.exec sandbox.cpp src/ffly_client.o -lsfml-graphics -lsfml-window -lsfml-system

server: src/ffly_server.o
	g++ -std=c++11 -o ffly_server.exec src/ffly_server.o -lboost_system -lboost_filesystem -lpthread -lboost_thread

clean:
	rm -f src/*.o
	rm -f *.exec

src/ffly_server.o: src/ffly_server.cpp
	g++ -c -std=c++11 -o src/ffly_server.o src/ffly_server.cpp

src/ffly_client.o: src/ffly_client.cpp
	g++ -c -std=c++11 -o src/ffly_client.o src/ffly_client.cpp

src/comm_deamon.o: src/comm_deamon.cpp
	g++ -c -std=c++11 -o src/comm_deamon.o src/comm_deamon.cpp
