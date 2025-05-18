#ifndef JEU_TICTACTOE_H
#define JEU_TICTACTOE_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


void initialiser();
void afficher_tictactoe();
void jouer_tictactoe(bool displayResult);
void ROTOzoom(SDL_Surface *image);
void jouer_multiple_parties();

#endif /* JEU_TICTACTOE_H */
