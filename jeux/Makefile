CC = gcc
CFLAGS = -g
LIBS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

prog: main.o menu.o text.o perso.o background.o enigme.o minimap.o xo.o
	$(CC) main.o menu.o text.o perso.o background.o enigme.o minimap.o xo.o -o prog $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

menu.o: menu.c
	$(CC) -c menu.c -o menu.o $(CFLAGS)

text.o: text.c
	$(CC) -c text.c -o text.o $(CFLAGS)
	
perso.o: perso.c
	$(CC) -c perso.c -o perso.o $(CFLAGS)

background.o: background.c
	$(CC) -c background.c -o background.o $(CFLAGS)
	
enigme.o: enigme.c
	$(CC) -c enigme.c -o enigme.o $(CFLAGS)
	
minimap.o: minimap.c
	$(CC) -c minimap.c -o minimap.o $(CFLAGS)
	
xo.o: xo.c
	$(CC) -c xo.c -o xo.o $(CFLAGS)
	
clean:
	rm -f *.o prog

