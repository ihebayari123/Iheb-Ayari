#ifndef PERSO_H
#define PERSO_H
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#define SCREEN_H 800
#define SCREEN_W 1200

typedef struct {
    SDL_Surface* idle;
    SDL_Surface* run;
    SDL_Surface* runleft;
    SDL_Surface* jump;
    SDL_Surface* jumpleft;
    SDL_Surface* current_sprite;
    int direction;
    int speed;
    int isJumping;
    int jumpSpeed;
    
    int ground;
    int move;
    
    SDL_Rect posScreen;
    SDL_Rect posSprite;
    SDL_Rect posSprite2;
    SDL_Rect posSprite3;
    SDL_Rect posSprite4;
    SDL_Rect posSprite5;
    
    
    
    int manette;
    int right ,left,upm,fight;
} Personne;

void init(Personne *p, int numperso);
void initPerso(Personne *p);
void afficherPerso(Personne p, SDL_Surface * screen);
void moveperso(Personne *p);
void animerperso(Personne* p);
void saut(Personne* p, int *flag);


#endif

