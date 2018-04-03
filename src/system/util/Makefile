#base64: clean
#	gcc -o main base64.c
test:
	gcc -c hash.c
	gcc test.c hash.o -pthread
hash:
	gcc hash.c
#	gcc -c checksum.c
#	gcc main.c hash.o checksum.o -o main
clean:
	rm -f main *.o
