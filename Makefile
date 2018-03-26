run:
	echo " " > debug.h
	gcc -c init.c createDisk.c main.c get.c read.c write.c  entry.c
	gcc -g init.o createDisk.o main.o get.o read.o write.o entry.o -lm
	./a.out vfs.bin

d:
	echo "#define DEBUG_BUILD" > debug.h
	gcc -c init.c createDisk.c main.c get.c read.c write.c  entry.c
	gcc -g init.o createDisk.o main.o get.o read.o write.o entry.o -lm
	./a.out vfs.bin

out:
	echo " " > debug.h
	gcc -c init.c createDisk.c main.c get.c read.c write.c  entry.c
	gcc -g init.o createDisk.o main.o get.o read.o write.o entry.o -lm
	./a.out vfs.bin > out.file
	rm a.out *.o
clean:
	rm a.out *.o
	rm *.bin
