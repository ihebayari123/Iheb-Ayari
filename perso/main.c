#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "perso.h"

#define SCREEN_H 800
#define SCREEN_W 1200

int main() {
    int flag = 0; // Initialize flag for key release event
    int jumpFlag = 0;
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE);
    if (screen == NULL) {
        printf("Screen surface could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    Personne perso;
    initPerso(&perso);

    SDL_Surface* image = IMG_Load("background.jpg");
    if (image == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Rect imagepos;

    imagepos.x = 0;
    imagepos.y = 0;
    int ctrl_pressed = 0;
    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                
             case SDL_KEYDOWN:
             
             if (event.key.keysym.sym == SDLK_RIGHT) {
             if (ctrl_pressed) {
                            perso.speed = 20;
                            perso.move = 1;
                
                if(perso.posScreen.x > SCREEN_W-60 )
                {
                 perso.move = 0;
                 }
                        } else {
                            perso.speed = 5;
                            perso.move = 1;
                
                if(perso.posScreen.x < 0)
                {
                 perso.move = 0;
                 }
                            }
              
              perso.direction =1;
              perso.current_sprite =perso.run;
             }
             if (event.key.keysym.sym == SDLK_LEFT) {
             if (ctrl_pressed) {
                            perso.speed = 20; 
                            perso.move = 1;
                
                if(perso.posScreen.x < 0)
                {
                 perso.move = 0;
                 }
                        } else {
                            perso.speed = 5;perso.move = 1;
                
                if(perso.posScreen.x < 0)
                {
                 perso.move = 0;
                 }
                 }
              
              perso.direction =-1;
              perso.current_sprite =perso.runleft;
             }
             if (event.key.keysym.sym == SDLK_SPACE) {
             
             perso.jumpSpeed = 22;
            
                            flag = 1;
                        
             }
             
             if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                 ctrl_pressed = 1;
                 }
             
             break;  
            case SDL_KEYUP:
            
             if (event.key.keysym.sym == SDLK_RIGHT) {
              perso.speed = 0;
              perso.direction =0;
              perso.move = 0;
              perso.current_sprite =perso.idle;
               }
               
             if (event.key.keysym.sym == SDLK_LEFT) {
              perso.speed = 0;
              perso.direction =0;
              perso.move = 0;
              perso.current_sprite =perso.idle;
               }  
               
             if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                 ctrl_pressed = 0;
                 }  
             if (event.key.keysym.sym == SDLK_SPACE) {
                 
                 flag = 0;
                 }
            break;
            }
        }
        
        SDL_BlitSurface(image,NULL,screen,&imagepos);
        
        moveperso(&perso);
        saut(&perso, &flag);
        
        animerperso(&perso);

        afficherPerso(perso, screen);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        
        SDL_Flip(screen);

       
        SDL_Delay(50);
    }

   
    SDL_FreeSurface(image);
    //SDL_FreeSurface(perso.idle);
    SDL_Quit();

    return 0;
}

