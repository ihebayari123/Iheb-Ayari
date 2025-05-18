CC = gcc
CFLAGS = -g
LIBS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

prog: main.o perso.o serie.o
	$(CC) main.o perso.o serie.o -o prog $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)
	
perso.o: perso.c
	$(CC) -c perso.c -o perso.o $(CFLAGS)
	
serie.o: serie.c
	$(CC) -c serie.c -o serie.o $(CFLAGS)
	
clean:
	rm -f *.o prog
