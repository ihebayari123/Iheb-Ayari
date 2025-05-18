CC = gcc
CFLAGS = -g
LIBS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

prog: main.o jeu_tictactoe.o 
	$(CC) main.o jeu_tictactoe.o -o prog $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)
	
perso.o: jeu_tictactoe.c
	$(CC) -c jeu_tictactoe.c -o jeu_tictactoe.o $(CFLAGS)
	
clean:
	rm -f *.o prog
