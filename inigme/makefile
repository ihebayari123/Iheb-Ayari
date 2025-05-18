CC = gcc
CFLAGS = -g
LIBS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

prog: main.o enigme.o
	$(CC) main.o enigme.o -o prog $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

enigme.o: enigme.c
	$(CC) -c enigme.c -o enigme.o $(CFLAGS)

clean:
	rm -f *.o prog

