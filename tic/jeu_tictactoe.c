
#include "jeu_tictactoe.h"

#define SCREEN_WIDTH 522
#define SCREEN_HEIGHT 540
#define SPRITE_SIZE 64

SDL_Surface *screen1 = NULL;
SDL_Surface *x_image = NULL;
SDL_Surface *o_image = NULL;
SDL_Surface *background = NULL;
SDL_Surface *win = NULL;
SDL_Surface *lose = NULL;
SDL_Surface *win1 = NULL;
SDL_Surface *lose1 = NULL;
SDL_Surface *win2 = NULL;
SDL_Surface *lose2 = NULL;


char grille[3][3];
bool joueur_actuel = true;

void initialiser() {
    SDL_Init(SDL_INIT_VIDEO);
    screen1 = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE); // Changed 'screen' to 'screen1'
    SDL_WM_SetCaption("Tic-Tac-Toe", NULL);
    x_image = IMG_Load("x.png");
    o_image = IMG_Load("o.png");
    win = IMG_Load("win.png");
    lose = IMG_Load("lose.png");
    win1 = IMG_Load("win1.png");
    lose1 = IMG_Load("lose1.png");
    win2 = IMG_Load("win2.png");
    lose2 = IMG_Load("lose2.png");

    background = IMG_Load("background.png");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            grille[i][j] = ' ';
        }
    }
}


void afficher_tictactoe() {
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    SDL_BlitSurface(background, NULL, screen1, &pos);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            SDL_Rect case_pos;
            case_pos.x = j * 180 + (180 - x_image->w) / 2;
            case_pos.y = i * 180 + (180 - x_image->h) / 2;
            if (grille[i][j] == 'X') {
                SDL_BlitSurface(x_image, NULL, screen1, &case_pos); // Changed 'screen' to 'screen1'
            } else if (grille[i][j] == 'O') {
                SDL_BlitSurface(o_image, NULL, screen1, &case_pos); // Changed 'screen' to 'screen1'
            }
        }
    }
    SDL_Flip(screen1);
}





bool verifier_victoire(char joueur) {
    // Vérifier les lignes
    for (int i = 0; i < 3; i++) {
        if (grille[i][0] == joueur && grille[i][1] == joueur && grille[i][2] == joueur) {
            return true;
        }
    }
    // Vérifier les colonnes
    for (int j = 0; j < 3; j++) {
        if (grille[0][j] == joueur && grille[1][j] == joueur && grille[2][j] == joueur) {
            return true;
        }
    }
    // Vérifier les diagonales
    if ((grille[0][0] == joueur && grille[1][1] == joueur && grille[2][2] == joueur) ||
        (grille[0][2] == joueur && grille[1][1] == joueur && grille[2][0] == joueur)) {
        return true;
    }
    return false;
}

bool grille_pleine() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}


void ROTOzoom(SDL_Surface *image) {
    if (image == NULL) {
        fprintf(stderr, "Image is NULL\n");
        return;
    }

    if (screen1 == NULL) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect destRect;

    destRect.x = 109.8;
    destRect.y = 178;
    destRect.w = SCREEN_WIDTH;
    destRect.h = SCREEN_HEIGHT;

    if (SDL_BlitSurface(image, NULL, screen1, &destRect) != 0) {
        fprintf(stderr, "Unable to blit image to screen: %s\n", SDL_GetError());
    }

    SDL_Flip(screen1);

    SDL_FreeSurface(screen1);
}
void jouer_tictactoe(bool displayResult) {
    bool fin_de_partie = false;
    while (!fin_de_partie) {
        afficher_tictactoe();
        SDL_Flip(screen1); // Update the screen
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                fin_de_partie = true;
            }
            if ((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN) && joueur_actuel) {
                int row, col;
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouse_x = event.button.x;
                    int mouse_y = event.button.y;
                    row = mouse_y / 180; // Calculate corresponding row
                    col = mouse_x / 180; // Calculate corresponding column
                } else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            row = 2; col = 0; break;
                        case SDLK_2:
                            row = 2; col = 1; break;
                        case SDLK_3:
                            row = 2; col = 2; break;
                        case SDLK_4:
                            row = 1; col = 0; break;
                        case SDLK_5:
                            row = 1; col = 1; break;
                        case SDLK_6:
                            row = 1; col = 2; break;
                        case SDLK_7:
                            row = 0; col = 0; break;
                        case SDLK_8:
                            row = 0; col = 1; break;
                        case SDLK_9:
                            row = 0; col = 2; break;
                        default:
                            continue; // Ignore other keys
                    }
                }
                if (grille[row][col] == ' ') {
                    grille[row][col] = 'X';
                    joueur_actuel = false;
                    if (verifier_victoire('X')) {
                        fin_de_partie = true;
                    } else if (grille_pleine()) {
                        fin_de_partie = true;
                    }
                }
            }
        }

        if (!joueur_actuel && !fin_de_partie) {
            // Computer plays
            int row, col;
            if (!grille_pleine()) {
                // Simplified strategy for computer, can be improved
                do {
                    row = rand() % 3;
                    col = rand() % 3;
                } while (grille[row][col] != ' ');
                grille[row][col] = 'O';
                joueur_actuel = true;
                if (verifier_victoire('O')) {
                    fin_de_partie = true;
                } else if (grille_pleine()) {
                    fin_de_partie = true;
                }
            }
        }
    }

    if (displayResult) {
        if (verifier_victoire('X')) {
            printf("You win!\n");
        } else if (verifier_victoire('O')) {
            printf("Computer wins!\n");
        } else {
            printf("It's a draw!\n");
        }
    }
  }
void jouer_multiple_parties() { //tache blanche
    int wins = 0, losses = 0;

    for (int i = 0; i < 3; i++) {
        initialiser();
        jouer_tictactoe(false);
        if (verifier_victoire('X')) {
            wins++;
        } else {
            losses++;
        }
    }

    if (wins >= 2) {
        ROTOzoom(win2);
        SDL_Delay(1000);
        SDL_FreeSurface(win2);
        ROTOzoom(win1);
        SDL_Delay(1000);
        SDL_FreeSurface(win1);
        ROTOzoom(win);
        SDL_Delay(1000);
        SDL_FreeSurface(win);
    } else {
      ROTOzoom(lose2);
      SDL_Delay(1000);
      SDL_FreeSurface(lose2);
      ROTOzoom(lose1);
      SDL_Delay(1000);
      SDL_FreeSurface(lose1);
      ROTOzoom(lose);
      SDL_Delay(1000);
      SDL_FreeSurface(lose);
    }
}
