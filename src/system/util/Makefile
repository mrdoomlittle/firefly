base64: clean
	gcc -o main base64.c
all:
	gcc -c hash.c
	gcc -c checksum.c
	gcc main.c hash.o checksum.o -o main
clean:
	rm -f main *.o
