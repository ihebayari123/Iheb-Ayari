#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "enigme.h"

int main() {
    int continuer = 1;
    
    enigme e,e1, e2, e3, e4;
    SDL_Event event;
    int test = 0;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    
    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    initanim(&e);
    InitEnigme(&e1, "niveau1.txt");
    InitEnigme(&e2, "niveau2.txt");
    InitEnigme(&e3, "niveau3.txt");
    InitEnigme(&e4, "niveau4.txt");

    afficherEnigme(e2, screen);

    while (continuer) {
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:    
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseY = event.button.y;
                        int mouseX = event.button.x;
                        if (mouseY >= 210 && mouseY < 220 && mouseX >= 130 && mouseX <=450) {
                            printf("Réponse correcte !\n");
                            test= 1;
                            
                        } else if (mouseY >= 250 && mouseY < 270 && mouseX >= 130 && mouseX <=400) {
                              printf("Mauvaise réponse.\n");
                              continuer = 0;
                          } else if (mouseY >= 310 && mouseY < 320 && mouseX >= 130 && mouseX <=350) {
                                printf("Mauvaise réponse.\n");
                                continuer = 0;
                            }
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {

                        continuer = 0;
                    break;
            }
        }
    }
    if (test == 1)
    animerEnig(&e,screen);

    }
    
    SDL_Quit();
    return 0;
    }

