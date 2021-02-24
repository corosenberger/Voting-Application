
CC=gcc
DelCom=del

remake: clean all

all: GarbageBin.o Garbage.o List.o String.o Build.o Crypto.o
	$(CC) -w `pkg-config --cflags gtk+-3.0` MainClient.c GarbageBin.o Garbage.o List.o String.o Build.o Crypto.o -lssl -lcrypto `pkg-config --libs gtk+-3.0` -o main.o

GarbageBin.o:
	$(CC) -c -w GarbageBin.c -o GarbageBin.o

Garbage.o: GarbageBin.o
	$(CC) -c Garbage.c -o Garbage.o

List.o:
	$(CC) -c List.c -o List.o

String.o:
	$(CC) -c String.c -o String.o

Build.o:
	$(CC) -w `pkg-config --cflags gtk+-3.0` -c Build.c `pkg-config --libs gtk+-3.0` -o Build.o

Crypto.o:
	$(CC) -c Crypto.c -lssl -lcrypto -o Crypto.o

clean:
	rm GarbageBin.o
	rm Garbage.o
	rm List.o
	rm String.o
	rm Build.o
	rm Crypto.o
	rm main.o
