base64: clean
	gcc -o main base64.c
all:
	gcc -c checksum.c
	gcc main.c checksum.o -o main
clean:
	rm -f main *.o
