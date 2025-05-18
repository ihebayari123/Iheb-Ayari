#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "background.h"
#include "score.h"

#define SINGLE_SCREEN_WIDTH 1500
#define SINGLE_SCREEN_HEIGHT 400
#define MULTI_SCREEN_HEIGHT 800 // Double de SINGLE_SCREEN_HEIGHT pour le mode multijoueur



bool keys[SDLK_LAST] = { false };

int main() {
    Back b3;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *screen = SDL_SetVideoMode(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_EnableKeyRepeat(60, 60);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

    Mix_Chunk *son_haut = Mix_LoadWAV("notification.wav");
    if (son_haut == NULL) {
        printf("Failed to load sound: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

     Background background, background2;
    initBack(&background);
    initanim(&b3);
    initBack(&background2);
    initBackPartage(&background2);

    TTF_Init();
    SDL_EnableKeyRepeat(250, 60);
    char nom[30];
    enterPlayerName(nom, screen);
    printf("%s\n", nom);

    SDL_Rect button_rect = { 10, 10, 0, 0 };
    SDL_Color textColor = {0, 0, 220};
    TTF_Font *font = TTF_OpenFont("arial.ttf", 28);
    SDL_Surface* button_surface = IMG_Load("b1.png");
    if (button_surface == NULL) {
        printf("Failed to load button image: %s\n", IMG_GetError());
        return 1;
    }
    button_rect.w = button_surface->w;
    button_rect.h = button_surface->h;

    Score score;
    strcpy(score.name, nom);
    score.score = 0;

    char ch_score[64];
    int quit = 0;
    bool multiplayerMode = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    keys[event.key.keysym.sym] = true;
                    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                        Mix_PlayChannel(-1, son_haut, 0);
                    } else if (event.key.keysym.sym == SDLK_m) {
                        multiplayerMode = !multiplayerMode;
                        if (multiplayerMode) {
                            screen = SDL_SetVideoMode(SINGLE_SCREEN_WIDTH, MULTI_SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                        } else {
                            screen = SDL_SetVideoMode(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                        }
                    }
                    break;
                case SDL_KEYUP:
                    keys[event.key.keysym.sym] = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT &&
                        event.button.x >= button_rect.x && event.button.x <= button_rect.x + button_rect.w &&
                        event.button.y >= button_rect.y && event.button.y <= button_rect.y + button_rect.h) {
                        show_top_scores(screen);
                    }
                    break;
            }
        }

        // Adjust score based on key presses
        if (keys[SDLK_LEFT]) {
            scrolling(&background, 1, 5);
        }
        if (keys[SDLK_RIGHT]) {
            scrolling(&background, 0, 5);
            score.score++;
        }
        if (keys[SDLK_UP]) {
            scrolling(&background, 2, 5);
            score.score++;
        }
        if (keys[SDLK_DOWN]) {
            scrolling(&background, 3, 5);
        }
        if (keys[SDLK_q]) {
            scrolling(&background2, 1, 5);
        }
        if (keys[SDLK_d]) {
            scrolling(&background2, 0, 5);
            score.score++;
        }
        if (keys[SDLK_z]) {
            scrolling(&background2, 2, 5);
            score.score++;
        }
        if (keys[SDLK_s]) {
            scrolling(&background2, 3, 5);
        }

        // Render backgrounds and text
        if (multiplayerMode) {
            afficherBackPartage(background, background2, screen);
            animerBack(&b3,screen,&background);
        } else {
            afficherBack(background, screen);
            animerBack(&b3,screen,&background);
        }

        // Render the button
        SDL_BlitSurface(button_surface, NULL, screen, &button_rect);

        sprintf(ch_score, "Score: %d", score.score);
        renderText(screen, font, textColor, ch_score, 450, 10);
        SDL_Flip(screen); 
    }

    SDL_FreeSurface(button_surface);
    SDL_FreeSurface(background.backgroundImg);
    SDL_FreeSurface(background2.backgroundImg);
    Mix_FreeChunk(son_haut);
    SDL_Quit();
    Mix_CloseAudio();

    save_score(score);
    return 0;
}


