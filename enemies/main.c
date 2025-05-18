#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include "collision.h"

#define MOVE_LEFT   1
#define MOVE_RIGHT  2
#define MOVE_UP     4
#define MOVE_DOWN   8
#define INITIAL_M2_X 500
#define INITIAL_M2_Y 200





int main(int argc, char** argv) {
    
    SDL_Surface* ecran;
    Background b2;
    Entity e;

    int quitter = 1;

    initBack(&b2);
    inite(&e,ecran);
  
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Echec d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    } else
        printf("Bonjour tout le monde, SDL initialisé avec succès\n");

    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (ecran == NULL) {
        fprintf(stderr, "Echec de création de la fenêtre : %s.\n", SDL_GetError());
        return 1;
    }

    SDL_BlitSurface(e.image_h1, NULL, ecran, &e.pos_ecran_h1);
    collisionTri(&e,ecran);

    while (quitter) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_d:
                        case SDLK_RIGHT: // Right arrow
                            e.movement_flags |= MOVE_RIGHT;
                            break;
                        case SDLK_q:
                        case SDLK_LEFT: // Left arrow
                            e.movement_flags |= MOVE_LEFT;
                            break;
                        case SDLK_z:
                        case SDLK_UP: // Top arrow
                            e.movement_flags |= MOVE_UP;
                            break;
                        case SDLK_s:
                        case SDLK_DOWN: // Bottom arrow
                            e.movement_flags |= MOVE_DOWN;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_d:
                        case SDLK_RIGHT: // Right arrow
                            e.movement_flags &= ~MOVE_RIGHT;
                            break;
                        case SDLK_q:
                        case SDLK_LEFT: // Left arrow
                            e.movement_flags &= ~MOVE_LEFT;
                            break;
                        case SDLK_z:
                        case SDLK_UP: // Top arrow
                            e.movement_flags &= ~MOVE_UP;
                            break;
                        case SDLK_s:
                        case SDLK_DOWN: // Bottom arrow
                            e.movement_flags &= ~MOVE_DOWN;
                            break;
                    }
                    break;
            }
        }

        if (!e.isGameOver) {
           move(&e);
           moveIA(&e);
           
            if (abs(e.pos_ecran_m1.x - e.pos_ecran_m2.x) <= 50 && 
    abs(e.pos_ecran_m1.y - e.pos_ecran_m2.y) <= 50) {
    // Code for dealing damage when image_m2 is around image_m1 by 50 pixels


                Uint32 current_time = SDL_GetTicks();

                if (current_time - e.last_touch_time > 1000) {
                    e.touch_counter++;
                    e.last_touch_time = current_time;

                    if (e.score >= 50) {
                        e.score -= 50;
                    } else {
                        e.score = 0;
                    }

                    if (e.touch_counter == 1) {
                        SDL_BlitSurface(e.image_h2, NULL, ecran, &e.pos_ecran_h1);
                        SDL_FreeSurface(e.image_h1);
                    }
                    else if (e.touch_counter == 2) {
                        SDL_BlitSurface(e.image_h3, NULL, ecran, &e.pos_ecran_h1);
                        SDL_FreeSurface(e.image_h2);
                    }
                    else if (e.touch_counter == 3) {
                        SDL_BlitSurface(e.image_h4, NULL, ecran, &e.pos_ecran_h1);
                        SDL_FreeSurface(e.image_h3);
                    }
                    else if (e.touch_counter == 4) {
                        SDL_Rect screen_rect = {0, 0, 800, 600};
                        SDL_BlitSurface(e.image_gov, NULL, ecran, &screen_rect);
                        SDL_Flip(ecran);
                        SDL_Delay(2000);
                        quitter = 0;
                        e.isGameOver = 1;
                    }
                }
            }

            if (e.pos_ecran_m1.x < e.pos_ecran_c1.x + e.image_c1->w &&
    e.pos_ecran_m1.x + e.image_m1->w > e.pos_ecran_c1.x &&
    e.pos_ecran_m1.y < e.pos_ecran_c1.y + e.image_c1->h &&
    e.pos_ecran_m1.y + e.image_m1->h > e.pos_ecran_c1.y) {
    if (!e.is_c1_hidden) {
        e.score += 50;
        e.is_c1_hidden = 1;
    }
}

            Uint32 current_time = SDL_GetTicks();
            if (!e.isGameOver && current_time - e.last_score_update_time >= 1000) {
                e.score += 10;
                e.last_score_update_time = current_time;
            }

            if (current_time - e.last_blink_time >= 500) {
                e.c1_visible = !e.c1_visible;
                e.last_blink_time = current_time;
            }
        }

        if (e.returning_to_initial) {
            if (e.pos_ecran_m2.x != INITIAL_M2_X) {
                if (e.pos_ecran_m2.x < INITIAL_M2_X) {
                    e.pos_ecran_m2.x += 1;
                } else {
                    e.pos_ecran_m2.x -= 1;
                }
            }
            if (e.pos_ecran_m2.y != INITIAL_M2_Y) {
                if (e.pos_ecran_m2.y < INITIAL_M2_Y) {
                    e.pos_ecran_m2.y += 1;
                } else {
                    e.pos_ecran_m2.y -= 1;
                }
            }
            if (e.pos_ecran_m2.x == INITIAL_M2_X && e.pos_ecran_m2.y == INITIAL_M2_Y) {
                e.returning_to_initial = 0;
            }
        } else {
            if (e.distance < 200) {
                if (e.pos_ecran_m2.x < e.pos_ecran_m1.x) {
                    e.pos_ecran_m2.x += 1;
                } else if (e.pos_ecran_m2.x > e.pos_ecran_m1.x) {
                    e.pos_ecran_m2.x -= 1;
                }
                if (e.pos_ecran_m2.y < e.pos_ecran_m1.y) {
                    e.pos_ecran_m2.y += 1;
                } else if (e.pos_ecran_m2.y > e.pos_ecran_m1.y) {
                    e.pos_ecran_m2.y -= 1;
                }
            } else {
                float distance_to_initial = sqrt(pow(e.pos_ecran_m2.x - INITIAL_M2_X, 2) + pow(e.pos_ecran_m2.y - INITIAL_M2_Y, 2));
                if (distance_to_initial > 50) {
                    e.returning_to_initial = 1;
                } else {
                    if (e.pos_ecran_m2.y <= 0 || e.pos_ecran_m2.y >= ecran->h - e.image_m2[0]->h) {
                        e.bounce_direction *= -1;
                    }
                    e.pos_ecran_m2.y += 2 * e.bounce_direction;
                }
            }
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(b2.image_back, NULL, ecran, &b2.pos_ecran_back);
       
        afficherEnnemi(e, ecran);
           

        collisionBB(e,ecran,b2);

        SDL_Flip(ecran);

        SDL_Delay(10);
    }


   FreeIMG(e,b2);
    

    return 0;
}

