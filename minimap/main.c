#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include "fonction.h"

// Define map boundaries
#define MAP_WIDTH 1500
#define MAP_HEIGHT 850

int main(int argc, char* argv[]) {
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Surface* screen = SDL_SetVideoMode(1500, 500, 32, SDL_SWSURFACE);
    Minimap minimap;
    init_minimap(&minimap);
  
    Uint32 start_time = SDL_GetTicks();
    
    SDL_Surface* background = IMG_Load("back.png");
    SDL_Surface* player = IMG_Load("perso.png");
    SDL_Surface* masque = IMG_Load("backmask.png");

    SDL_Rect player_pos = {10, 150, 0, 0};

    // Flags for movement
    int moving_up = 0, moving_down = 0, moving_left = 0, moving_right = 0;
   
    SDL_Event event;
    int running = 1;
    while (running) {
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            moving_up = 1;
                            break;
                        case SDLK_DOWN:
                            moving_down = 1;
                            break;
                        case SDLK_LEFT:
                            moving_left = 1;
                            break;
                        case SDLK_RIGHT:
                            moving_right = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            moving_up = 0;
                            break;
                        case SDLK_DOWN:
                            moving_down = 0;
                            break;
                        case SDLK_LEFT:
                            moving_left = 0;
                            break;
                        case SDLK_RIGHT:
                            moving_right = 0;
                            break;
                    }
                    break;
            }
        }

        // Update player position based on movement flags
        if (moving_up && player_pos.y > 0) {
            player_pos.y -= 10;
            if (collisionPP(player_pos, masque)) {
                player_pos.y += 10; 
            }
        }
        if (moving_down && player_pos.y < MAP_HEIGHT - player_pos.h) {
            player_pos.y += 10;
            if (collisionPP(player_pos, masque)) {
                player_pos.y -= 10;
            }
        }
        if (moving_left && player_pos.x > 0) {
            player_pos.x -= 10;
            if (collisionPP(player_pos, masque)) {
                player_pos.x += 10;
            }
        }
        if (moving_right && player_pos.x < MAP_WIDTH - player_pos.w) {
            player_pos.x += 10;
            if (collisionPP(player_pos, masque)) {
                player_pos.x -= 10;
            }
        }
        
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time = current_time - start_time;

        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_BlitSurface(player, NULL, screen, &player_pos);

        // Update minimap
        MAJMinimap(player_pos, &minimap, player_pos, 11, MAP_WIDTH, MAP_HEIGHT);
        
        // Display minimap, time, and animations
        afficher_minimap(minimap, screen);
        affichertempsen(elapsed_time / 1000);
        affichertemps(elapsed_time / 1000);
        animerMinimap(&minimap);
        animerMinimape(&minimap);
        animerMinimape1(&minimap);
        
        SDL_Flip(screen);
    }

    // Free resources and quit
    SDL_FreeSurface(background);
    SDL_FreeSurface(player);
    SDL_FreeSurface(masque);
    liberer_minimap(&minimap);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

