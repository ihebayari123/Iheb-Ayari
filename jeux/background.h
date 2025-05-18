#ifndef JEUX_H_INCLUDED
#define JEUX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

typedef struct 
{
    SDL_Surface *backgroundImg;
    SDL_Surface *perso;
    SDL_Rect camera, rectperso;
    SDL_Rect pos;
    
} Background;

typedef struct
{
SDL_Surface *anim[5];
SDL_Surface *anim1[5];
SDL_Surface *anim2[5];
SDL_Surface* NPC_idle[8];
SDL_Rect animpos;
SDL_Rect NPCpos;
SDL_Rect NPCfixed;
SDL_Surface* introImage;

}Back;
void initBack(Background *b);
void initBackPartage(Background *b);
void afficherBack(Background b, SDL_Surface *screen);
void afficherBackPartage(Background b, Background b2, SDL_Surface *screen);
void afficherBack1(Background b, SDL_Surface *screen);
void afficherBack2(Background b2, SDL_Surface *screen);
void scrolling(Background *b, int direction, int pas);
void initanim(Back *b);
void animerBack(Back *b,SDL_Surface *screen,Background *b1);


#endif // JEUX_H_INCLUDED

