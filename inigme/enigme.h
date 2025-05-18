
#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>
typedef struct {
    char question[100];
    char reponses[3][50]; 
    int solution;
    SDL_Surface *anim[3];
    SDL_Surface *bgenigme;
} enigme;
void initanim(enigme *e);
void afficherEnigme(enigme e, SDL_Surface *screen);
void InitEnigme(enigme *e, const char *nomFichier);
void animerEnig(enigme *e,SDL_Surface *screen);
#endif
