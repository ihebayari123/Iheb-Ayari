CC = gcc
CFLAGS = -g
LIBS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lm

prog: main.o collision.o
	$(CC) main.o collision.o -o prog $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)
	
collision.o: collision.c
	$(CC) -c collision.c -o collision.o $(CFLAGS)
	
clean:
	rm -f *.o prog 

