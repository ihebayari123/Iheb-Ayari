#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "background.h"
#include <stdbool.h> // Add this line
#define SINGLE_SCREEN_WIDTH 1500
#define SINGLE_SCREEN_HEIGHT 400
#define MULTI_SCREEN_HEIGHT 800

void initBack(Background *b) {
    b->backgroundImg = IMG_Load("bg3.png");
    if (b->backgroundImg == NULL) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        exit(1);
    }

    b->camera.x = 0;
    b->camera.y = 0;
    b->camera.w = SINGLE_SCREEN_WIDTH;
    b->camera.h = SINGLE_SCREEN_HEIGHT;

    b->musique = Mix_LoadMUS("1.mp3");
    if (b->musique == NULL) {
        fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(b->musique, -1);
    }
}

void initBackPartage(Background *b){
    b->camera.x = 0;
    b->camera.y = 0;
    b->camera.w = SINGLE_SCREEN_WIDTH;
    b->camera.h = SINGLE_SCREEN_HEIGHT;
}

void afficherBack(Background b, SDL_Surface *screen) {
    int numRepeatsX = (SINGLE_SCREEN_WIDTH / b.camera.w) + 1;
    int numRepeatsY = (500 / b.camera.h) + 1;

    for (int i = 0; i < numRepeatsX; i++) {
        for (int j = 0; j < numRepeatsY; j++) {
            SDL_Rect destRect = { i * b.camera.w, j * b.camera.h, b.camera.w, b.camera.h };
            SDL_BlitSurface(b.backgroundImg, &(b.camera), screen, &destRect);
        }
    }
}


void afficherBackPartage(Background b, Background b2, SDL_Surface *screen) {
    SDL_Rect topViewport = {0, 0, SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT};
    SDL_Rect bottomViewport = {0, SINGLE_SCREEN_HEIGHT, SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT};

    SDL_BlitSurface(b.backgroundImg, &b.camera, screen, &topViewport);
    SDL_BlitSurface(b2.backgroundImg, &b2.camera, screen, &bottomViewport);
}


void scrolling(Background *b, int direction, int pas) {
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

    if (camera->y < -5) camera->y = -5;
    if (camera->y > 20) camera->y = 20;
    if (camera->x < 0) camera->x = 0;
    if (camera->x > b->backgroundImg->w - SINGLE_SCREEN_WIDTH) {
        camera->x = b->backgroundImg->w - SINGLE_SCREEN_WIDTH;
    }
}

void initanim(Back *b){
    b->anim[0] = IMG_Load("advertising/1.png");
    b->anim[1] = IMG_Load("advertising/2.png");
    b->anim[2] = IMG_Load("advertising/3.png");
    b->anim[3] = IMG_Load("advertising/4.png");
    b->anim[4] = IMG_Load("advertising/5.png");

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
}

void animerBack(Back *b, SDL_Surface *screen, Background *b1) {
    int start = SDL_GetTicks();
    static int end = 0;
    static int ppp = 0;
    static int ppp2 = 0;
    static int ppp3 = 0;
    static int x = 0;
    int start2 = SDL_GetTicks();
    static int end2 = 0;
    if (start - end >= 350) {
        x = (x + 1) % 8;
        end = start;
    }
    if (start2 - end2 >= 500) {
        ppp = (ppp + 1) % 4;
        ppp2 = (ppp2 + 1) % 4;
        ppp3 = (ppp3 + 1) % 4;
        end2 = start2;
    }

    int fixedX = 823;
    int fixedY = 290;
    int fixedX2 = 20;
    int fixedY2 = 130;
    int fixedX3 = 1203;
    int fixedY3 = 25;
    int fixedX4 = 2423;
    int fixedY4 = 240;
    int fixedX5 = 2643;
    int fixedY5 = 240;

    SDL_Rect destRect = { fixedX - b1->camera.x, fixedY - b1->camera.y, 0, 0 };
    SDL_Rect destRect3 = { fixedX2 - b1->camera.x, fixedY2 - b1->camera.y, 0, 0 };
    SDL_Rect destRect4 = { fixedX3 - b1->camera.x, fixedY3 - b1->camera.y, 0, 0 };
    SDL_Rect destRect5 = { fixedX4 - b1->camera.x, fixedY4 - b1->camera.y, 0, 0 };
    SDL_Rect destRect6 = { fixedX5 - b1->camera.x, fixedY5 - b1->camera.y, 0, 0 };

    SDL_BlitSurface(b->anim[ppp], NULL, screen, &destRect);
    SDL_BlitSurface(b->anim1[ppp2], NULL, screen, &destRect3);
    SDL_BlitSurface(b->anim2[ppp3], NULL, screen, &destRect4);
    SDL_BlitSurface(b->anim[ppp], NULL, screen, &destRect5);
    SDL_BlitSurface(b->anim1[ppp2], NULL, screen, &destRect6);
}

