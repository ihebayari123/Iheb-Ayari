#ifndef COLLISION_H
#define COLLISION_H
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

typedef struct {

SDL_Surface* image_back;
SDL_Rect pos_ecran_back;

}Background;

typedef struct {

    SDL_Surface* image_m1;
    SDL_Surface* image_m2[5];
    SDL_Surface* image_h1;
    SDL_Surface* image_h2;
    SDL_Surface* image_h3;
    SDL_Surface* image_h4;
    SDL_Rect pos_ecran_m1;
    SDL_Rect pos_ecran_m2;
    SDL_Rect pos_ecran_h1;
    SDL_Surface* image_c1;
    SDL_Rect pos_ecran_c1;
    SDL_Surface* image_gov;
    
    TTF_Font* font;
    SDL_Color textColor;
    float distance;
    int isGameOver;
    int movement_flags;
    int score;
    int touch_counter;
    
    Uint32 last_touch_time;
    
    int is_c1_hidden;
    int c1_visible;
    int bounce_direction;
    int returning_to_initial;
    
     Uint32 last_score_update_time;
     Uint32 last_blink_time;
     char score_text[20];

}Entity;


void initBack(Background *b);
void inite(Entity *e,SDL_Surface* ecran);
void afficherEnnemi(Entity e, SDL_Surface  * screen);
void collisionTri( Entity *e,SDL_Surface  * ecran);
void move( Entity * e);
void collisionBB( Entity e,SDL_Surface  * ecran,Background b2) ;
void moveIA( Entity * e);
SDL_Rect generateRandomPosition(int min_x, int max_x, int min_y, int max_y);
void FreeIMG(Entity e,Background b2);


#endif
