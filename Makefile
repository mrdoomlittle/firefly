CXXFLAGS=
LDFLAGS=
LLFLAGS=
all:

debug: src/comm_deamon.o
	g++ -std=c++11 -o sandbox.exec sandbox.cpp src/comm_deamon.o

clean:
	rm src/*.o
	rm *.exec

src/comm_deamon.o: src/comm_deamon.cpp
	g++ -c -std=c++11 $(LDFLAGS) $(CXXFLAGS) -o src/comm_deamon.o src/comm_deamon.cpp $(LLFLAGS)
