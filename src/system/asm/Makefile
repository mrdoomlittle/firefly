all: clean
	nasm -f elf64 -o sys_clone.o.0 sys_clone.asm
	gcc -c -o sys_clone.o.1 ../sys_clone.c

	nasm -f elf64 -o sys_getpid.o.0 sys_getpid.asm
	gcc -c -o sys_getpid.o.1 ../sys_getpid.c

	nasm -f elf64 -o sys_fork.o.0 sys_fork.asm
	gcc -c -o sys_fork.o.1 ../sys_fork.c

	nasm -f elf64 -o sys_kill.o.0 sys_kill.asm
	gcc -c -o sys_kill.o.1 ../sys_kill.c

	nasm -f elf64 -o sys_exit.o.0 sys_exit.asm
	gcc -c -o sys_exit.o.1 ../sys_exit.c

	gcc -c -o mutex.o.1 ../mutex.c
	nasm -f elf64 -o mutex.o.0 mutex.asm
	gcc main.c mutex.o.0 mutex.o.1 sys_fork.o.0 sys_fork.o.1 sys_kill.o.0 \
	sys_kill.o.1 sys_exit.o.0 sys_exit.o.1 sys_getpid.o.0 sys_getpid.o.1 \
	sys_clone.o.0 sys_clone.o.1
clean:
	rm -f *.o *.o*
