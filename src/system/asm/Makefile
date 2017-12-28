all: clean
	gcc -c -o mutex.o.1 ../mutex.c
	nasm -f elf64 -o mutex.o.0 mutex.asm
	gcc main.c mutex.o.0 mutex.o.1
clean:
	rm -f *.o *.o*
