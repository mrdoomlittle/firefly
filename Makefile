CXXFLAGS=
LDFLAGS=
LLFLAGS=
all:

sandbox: src/ffly_client.o
	g++ -std=c++11 -o sandbox.exec sandbox.cpp src/ffly_client.o -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm src/*.o
	rm *.exec

src/ffly_client.o: src/ffly_client.cpp
	g++ -c -std=c++11 $(LDFLAGS) $(CXXFLAGS) -o src/ffly_client.o src/ffly_client.cpp $(LLFLAGS)

src/comm_deamon.o: src/comm_deamon.cpp
	g++ -c -std=c++11 $(LDFLAGS) $(CXXFLAGS) -o src/comm_deamon.o src/comm_deamon.cpp $(LLFLAGS)
