#ifndef PERSO_H
#define PERSO_H
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#define SCREEN_H 800
#define SCREEN_W 1200
#include "background.h"
#include "minimap.h"

typedef struct {
    SDL_Surface* idle;
    SDL_Surface* run;
    SDL_Surface* runleft;
    SDL_Surface* jump;
    SDL_Surface* jumpleft;
    SDL_Surface* doublejump;
    SDL_Surface* doublejumpleft;
    
    SDL_Surface* idle2;
    SDL_Surface* run2;
    SDL_Surface* runleft2;
    SDL_Surface* jump2;
    SDL_Surface* jumpleft2;
    SDL_Surface* doublejump2;
    SDL_Surface* doublejumpleft2;
    
    
    SDL_Surface* current_sprite;
    
    SDL_Surface* current_sprite2;
    
    int direction;
    int speed;
    int isJumping;
    int jumpSpeed;
    
    int jumpCount;
    
    int ground;
    int move;
    
    int numperso;
    SDL_Rect posScreen;
    SDL_Rect posSprite;
    SDL_Rect posSprite2;
    SDL_Rect posSprite3;
    SDL_Rect posSprite4;
    SDL_Rect posSprite5;
    SDL_Rect posSprite6;
    SDL_Rect posSprite7;
    
    
    SDL_Rect posSpritec;
    SDL_Rect posSprite2c;
    SDL_Rect posSprite3c;
    SDL_Rect posSprite4c;
    SDL_Rect posSprite5c;
    SDL_Rect posSprite6c;
    SDL_Rect posSprite7c;
    
    
    
} Personne;

void init(Personne *p, int numperso);
void initPerso(Personne *p);

void afficherPerso(Personne p, SDL_Surface * screen,int choix);

void moveperso(Personne *p, Background *bg, SDL_Surface* mask);
void animerperso(Personne* p,int choix);
void saut(Personne* p, int *flag,int choix);



#endif

