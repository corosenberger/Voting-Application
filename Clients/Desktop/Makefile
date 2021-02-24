
CC=gcc
DelCom=del

remake: clean all

all: GarbageBin.o Garbage.o List.o String.o Vote.o Crypto.o
	$(CC) -w `pkg-config --cflags gtk+-3.0` MainClient.c GarbageBin.o Garbage.o List.o String.o Vote.o Crypto.o -lssl -lcrypto `pkg-config --libs gtk+-3.0` -o main.o

wall: GarbageBin.o Garbage.o List.o String.o winVote.o Crypto.o
	$(CC) -w -pthread -I C:\\msys64\\mingw64\\include\\gtk-3.0 -I C:\\msys64\\mingw64\\include\\atk-1.0 -I C:\\msys64\\mingw64\\include\\cairo -I C:\\msys64\\mingw64\\include\\pango-1.0 -I C:\\msys64\\mingw64\\include\\glib-2.0 -I C:\\msys64\\mingw64\\lib\\glib-2.0\\include -I C:\\msys64\\mingw64\\include\\gdk-pixbuf-2.0 -I C:\\msys64\\mingw64\\include\\harfbuzz MainClient.c GarbageBin.o Garbage.o List.o String.o winVote.o Crypto.o -lssl -lcrypto -lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 -lgio-2.0 -lpangowin32-1.0 -lgdi32 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lcairo -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lglib-2.0 -lintl -o main.o

GarbageBin.o:
	$(CC) -c -w GarbageBin.c -o GarbageBin.o

Garbage.o: GarbageBin.o
	$(CC) -c Garbage.c -o Garbage.o

List.o:
	$(CC) -c List.c -o List.o

String.o:
	$(CC) -c String.c -o String.o

Vote.o:
	$(CC) -w `pkg-config --cflags gtk+-3.0` -c Vote.c `pkg-config --libs gtk+-3.0` -o Vote.o

winVote.o:
	$(CC) -w -pthread -I C:\\msys64\\mingw64\\include\\gtk-3.0 -I C:\\msys64\\mingw64\\include\\atk-1.0 -I C:\\msys64\\mingw64\\include\\cairo -I C:\\msys64\\mingw64\\include\\pango-1.0 -I C:\\msys64\\mingw64\\include\\glib-2.0 -I C:\\msys64\\mingw64\\lib\\glib-2.0\\include -I C:\\msys64\\mingw64\\include\\gdk-pixbuf-2.0 -I C:\\msys64\\mingw64\\include\\harfbuzz -c Vote.c -lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 -lgio-2.0 -lpangowin32-1.0 -lgdi32 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lcairo -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lglib-2.0 -lintl -o winVote.o

Crypto.o:
	$(CC) -c Crypto.c -lssl -lcrypto -o Crypto.o

clean:
	rm GarbageBin.o
	rm Garbage.o
	rm List.o
	rm String.o
	rm Vote.o
	rm Crypto.o
	rm main.o
