
CC=gcc
DelCom=rm

gtkFlags=`pkg-config --cflags gtk+-3.0`
gtkLibs=`pkg-config --libs gtk+-3.0`
cryptoFlags=-lssl -lcrypto

ifeq ($(OS),Windows_NT)
sockFlags=-lws2_32
else
sockFlags=
endif

localLibs=GarbageBin.o Garbage.o List.o String.o Build.o Crypto.o Client.o

output=main.o

remake: clean all

all: $(localLibs)
	$(CC) -w $(gtkFlags) MainClient.c $(localLibs) $(cryptoFlags) $(gtkLibs) -o $(output) $(sockFlags)

GarbageBin.o:
	$(CC) -c -w GarbageBin.c -o GarbageBin.o

Garbage.o:
	$(CC) -c Garbage.c -o Garbage.o

List.o:
	$(CC) -c List.c -o List.o

String.o:
	$(CC) -c String.c -o String.o

Build.o:
	$(CC) -w $(gtkFlags) -c Build.c $(gtkLibs) -o Build.o

Crypto.o:
	$(CC) -c Crypto.c $(cryptoFlags) -o Crypto.o

Client.o:
	$(CC) -pthread -c Client.c -o Client.o $(sockFlags)

clean:
	$(DelCom) $(localLibs) $(output)
