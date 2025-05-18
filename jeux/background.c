#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "background.h"


#define SCREEN_WIDTH 1120
#define SCREEN_HEIGHT 800

void initBack(Background *b) {	   
    
    // Load background image
    b->backgroundImg = IMG_Load("bg3.png");
    if (b->backgroundImg == NULL) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        exit(1);
    }
    
    // Set character position
    b->rectperso.x = 0;
    b->rectperso.y = 500;
    
    // Initialize camera dimensions
    b->camera.x = 0;
    b->camera.y = 0;
    b->camera.w = SCREEN_WIDTH;  
    b->camera.h = SCREEN_HEIGHT; 
    
    // Load music
    Mix_Music *musique;
    musique = Mix_LoadMUS("son.mp3");
    if (musique == NULL) {
        fprintf(stderr, "Echec de chargement de la musique : %s\n", Mix_GetError());
    }
    Mix_PlayMusic(musique, -1);
}

void initBackPartage(Background *b){
    // Set position for shared background
    b->pos.x = SCREEN_WIDTH / 2;
    b->pos.y = 0;
}

void afficherBack(Background b, SDL_Surface *screen) {
    int numRepeatsX = (SCREEN_WIDTH / b.camera.w) + 1;
    
    // Calculate the number of times the background image should be repeated vertically
    int numRepeatsY = (SCREEN_HEIGHT / b.camera.h) + 1;
    
    // Render the background image repeatedly to cover the entire screen
    for (int i = 0; i < numRepeatsX; i++) {
        for (int j = 0; j < numRepeatsY; j++) {
            SDL_Rect destRect = { i * b.camera.w, j * b.camera.h, b.camera.w, b.camera.h };
            SDL_BlitSurface(b.backgroundImg, &(b.camera), screen, &destRect);
        }
    }
}

void afficherBackPartage(Background b, Background b2, SDL_Surface *screen) {
    // Blit both background images and characters onto the screen
    SDL_BlitSurface(b.backgroundImg, &b.camera, screen, NULL);
    //SDL_BlitSurface(b.perso, NULL, screen, &(b.rectperso));
    SDL_BlitSurface(b2.backgroundImg, &b2.camera, screen, &(b2.pos));
    //SDL_BlitSurface(b2.perso, NULL, screen, &(b2.rectperso));
}

void scrolling(Background *b, int direction, int pas) {
    // Perform scrolling based on direction
    SDL_Rect *camera = &(b->camera); 
    if (direction == 0) { 
        camera->x += pas;
    } else if (direction == 1) { 
        camera->x -= pas;
    } else if (direction == 2) { 
        camera->y -= pas;
    } else if (direction == 3) { 
        camera->y += pas;
    }
    
    // Adjust camera boundaries
    if (camera->y < -5) camera->y = -5;
    if (camera->y > 20) camera->y = 20;
    if (camera->x < 0) camera->x = 0;
    
    // Limit horizontal scrolling to a maximum value of 2800
    if (camera->x > 2780 - SCREEN_WIDTH) {
        camera->x = 2780 - SCREEN_WIDTH;
    }
}

void initanim(Back *b){

 b->anim[0] = IMG_Load("advertising/1.png");
 b->anim[1] = IMG_Load("advertising/2.png");
 b->anim[2] = IMG_Load("advertising/3.png");
 b->anim[3] = IMG_Load("advertising/4.png");
 b->anim[4] = IMG_Load("advertising/5.png");

 
 char imagePath[100];
    for (int i = 0; i < 8; ++i) {
        sprintf(imagePath, "NPC1/Idle_0%d.png", i + 1);
        b->NPC_idle[i] = IMG_Load(imagePath);
        }
 

 b->anim1[0] = IMG_Load("advertising/6.png");
 b->anim1[1] = IMG_Load("advertising/7.png");
 b->anim1[2] = IMG_Load("advertising/8.png");
 b->anim1[3] = IMG_Load("advertising/9.png");
 b->anim1[4] = IMG_Load("advertising/10.png");
 
 b->anim2[0] = IMG_Load("advertising/11.png");
 b->anim2[1] = IMG_Load("advertising/12.png");
 b->anim2[2] = IMG_Load("advertising/13.png");
 b->anim2[3] = IMG_Load("advertising/14.png");
 b->anim2[4] = IMG_Load("advertising/15.png");
        
    b->animpos.x = 200;
    b->animpos.y = 200;
 
    b->NPCpos.x = 400;
    b->NPCpos.y = 400;
    
    
    b->NPCfixed.x = 900;
    b->NPCfixed.y = 360;

}

void animerBack(Back *b, SDL_Surface *screen,Background *b1) {
    int start = SDL_GetTicks();
    static int end = 0;
    static int ppp = 0;
    static int ppp2 = 0;
    static int ppp3 = 0;
    static int x = 0;
    int start2 = SDL_GetTicks();
    static int end2 = 0;
    if (start - end >= 350) {

        x = (x+1) % 8;
        end = start;
    }
    
        if (start2 - end2 >= 500) {
        ppp = (ppp + 1) % 4;
        ppp2 = (ppp2 + 1) % 4;
        ppp3 = (ppp3 + 1) % 4;
        end2 = start2;
    }
    
    

    // Fixed position on the background image
    int fixedX = 800;
    int fixedY = 290;
    
    int fixedX2 = 1;
    int fixedY2 = 130;
    
    int fixedX3 = 1150;
    int fixedY3 = 25;
    
    int fixedX4 = 2400;
    int fixedY4 = 240;
    
    int fixedX5 = 2620;
    int fixedY5 = 240;
    
    

    // Blit the animation at the fixed position on the screen
    SDL_Rect destRect = { fixedX - b1->camera.x, fixedY - b1->camera.y, 0, 0 };
    SDL_Rect destRect2 = { b->NPCfixed.x - b1->camera.x, b->NPCfixed.y - b1->camera.y, 0, 0 };
    SDL_Rect destRect3 = { fixedX2 - b1->camera.x, fixedY2 - b1->camera.y, 0, 0 };
    SDL_Rect destRect4 = { fixedX3 - b1->camera.x, fixedY3 - b1->camera.y, 0, 0 };
    SDL_Rect destRect5 = { fixedX4 - b1->camera.x, fixedY4 - b1->camera.y, 0, 0 };
    SDL_Rect destRect6 = { fixedX5 - b1->camera.x, fixedY5 - b1->camera.y, 0, 0 };
    
    SDL_BlitSurface(b->anim[ppp], NULL, screen, &destRect);
    SDL_BlitSurface(b->NPC_idle[x], NULL, screen, &destRect2);
    SDL_BlitSurface(b->anim1[ppp2], NULL, screen, &destRect3);
    SDL_BlitSurface(b->anim2[ppp3], NULL, screen, &destRect4);
    SDL_BlitSurface(b->anim[ppp], NULL, screen, &destRect5);
    SDL_BlitSurface(b->anim1[ppp2], NULL, screen, &destRect6);
}



