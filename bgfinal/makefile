prog:main.o score.o background.o
	gcc main.o -o prog background.c score.c -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -g
main.o:main.c
	gcc -c main.c -g
game.o:background.c
	gcc -c background.c -g
score.o:score.c
	gcc -c score.c -g


