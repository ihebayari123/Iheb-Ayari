#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#define MAX_NAME_LENGTH 20
#define MAX_SCORES 3
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 300

typedef struct ScoreInfo {
    int score;
    int temps;
    char playerName[MAX_NAME_LENGTH];
    SDL_Surface *scoreSurface;
    SDL_Rect scorePosition;
} ScoreInfo;

void saveScore(ScoreInfo s, char *fileName) {
    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }
    fprintf(file, "%d %d %s\n", s.score, s.temps, s.playerName);
    fclose(file);
}

void bestScore(char *filename, ScoreInfo trois[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    int i = 0;
    while (fscanf(file, "%d %d %s\n", &trois[i].score, &trois[i].temps, trois[i].playerName) == 3 && i < MAX_SCORES) {
        i++;
    }

    fclose(file);

    for (int i = 0; i < MAX_SCORES - 1; i++) {
        for (int j = i + 1; j < MAX_SCORES; j++) {
            if (trois[j].score > trois[i].score ||
                (trois[j].score == trois[i].score && trois[j].temps < trois[i].temps)) {
                ScoreInfo tmp = trois[i];
                trois[i] = trois[j];
                trois[j] = tmp;
            }
        }
    }
}

void afficherBest(SDL_Surface *screen, TTF_Font *font, ScoreInfo t[])
{
    SDL_Color color = {255, 255, 255};

    for (int i = 0; i < MAX_SCORES; i++) {
        char scoreText[50];
        sprintf(scoreText, "%s %d %d", t[i].playerName, t[i].score, t[i].temps);
        t[i].scoreSurface = TTF_RenderText_Solid(font, scoreText, color);
        if (t[i].scoreSurface == NULL) {
            printf("Erreur lors de la création de la surface de texte\n");
            break;
        }

        t[i].scorePosition.x = 100;
        t[i].scorePosition.y = 100 + i * 50;
        SDL_BlitSurface(t[i].scoreSurface, NULL, screen, &t[i].scorePosition);
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur lors de l'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (screen == NULL) {
        printf("Erreur lors de la création de la surface d'écran: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 20);
    if (font == NULL) {
        printf("Erreur lors du chargement de la police de caractères: %s\n", TTF_GetError());
        return 1;
    }

    ScoreInfo trois[MAX_SCORES];
    bestScore("scores.txt", trois);

    // Your game logic here

    // End of game: Save score
    ScoreInfo currentScore;
    currentScore.score = 1000; // Example score
    currentScore.temps = 50; // Example time
    strcpy(currentScore.playerName, "Player1"); // Example player name
    saveScore(currentScore, "scores.txt");

    // Display best scores at the end of the game
    afficherBest(screen, font, trois);


    // Free SDL resources, close file, etc.

    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();
    return 0;
}


