CFLAGS=-w -O2 -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
prog: fonction.o main.o
	gcc fonction.o main.o -o prog $(CFLAGS)
main.o: main.c fonction.h
	gcc -c main.c 
fonction.o: fonction.c fonction.h
	gcc -c fonction.c 
clean:
	rm -rf *.o prog
