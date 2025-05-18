#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <stdbool.h>

typedef struct {
    SDL_Surface *backgroundImg;
    SDL_Rect camera;
    Mix_Music *musique;
    SDL_Rect pos;
} Background;

typedef struct {
    SDL_Surface *anim[5];
    SDL_Surface *anim1[5];
    SDL_Surface *anim2[5];
    SDL_Rect animpos;
} Back;

void initBack(Background *b);
void initBackPartage(Background *b);
void afficherBack(Background b, SDL_Surface *screen);
void afficherBackPartage(Background b, Background b2, SDL_Surface *screen);
void scrolling(Background *b, int direction, int pas);
void initanim(Back *b);
void animerBack(Back *b, SDL_Surface *screen, Background *b1);

#endif // BACKGROUND_H

