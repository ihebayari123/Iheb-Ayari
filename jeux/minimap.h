#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    SDL_Surface* mini_image;
    SDL_Rect mini_position;
    SDL_Surface* player_image;
    SDL_Surface* player_image2;
    SDL_Rect player_position;
    SDL_Rect minimap_player_position; 
    SDL_Surface* masque;
    int numpersoo;
} Minimap;



void init_minimap(Minimap* m);
void MAJMinimap(SDL_Rect posJoueur,  Minimap * m, SDL_Rect camera, int redimensionnement, int mapWidth, int mapHeight);
void afficher_minimap(Minimap m, SDL_Surface* screen,int choix);
void liberer_minimap(Minimap* m);

SDL_Color GetPixel(SDL_Surface *mask,int x,int y);
int collisionPP(SDL_Rect player, SDL_Surface* mask);



void affichertemps(int temps);
void affichertempsen(Uint32 startTime);
void animerMinimap(Minimap* m);
void animerMinimape(Minimap* m);
void animerMinimape1(Minimap* m);




#endif


