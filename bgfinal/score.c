#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

// Comparison function for qsort
int compare_scores(const void *a, const void *b) {
    const Score *score1 = (const Score *)a;
    const Score *score2 = (const Score *)b;

    // Compare scores in descending order
    return score2->score - score1->score;
}

void save_score(Score score) {
    FILE *fp;
    fp = fopen("scores.txt", "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }
    fprintf(fp, "%s %d\n", score.name, score.score);
    fclose(fp);
}

void renderText(SDL_Surface *surface, TTF_Font *font, SDL_Color color, const char *text, int x, int y) {
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, color);
    SDL_BlitSurface(textSurface, NULL, surface, &destRect);
    SDL_FreeSurface(textSurface);
}

void show_top_scores(SDL_Surface *screen) {
    TTF_Font *font = TTF_OpenFont("arial.ttf", 28);
    if (font == NULL) {
        printf("Error opening font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {224, 176, 255}; // Mauve color

    char scoreStr[50];
    Score scores[100] = {0};
    int i;

    FILE *fp = fopen("scores.txt", "r");
    if (fp == NULL) {
        printf("Error opening file: scores.txt\n");
        TTF_CloseFont(font);
        return;
    }

    for (i = 0; i < 100; i++) {
        if (fscanf(fp, "%s %d", scores[i].name, &scores[i].score) == EOF) {
            break;
        }
    }
    fclose(fp);

    // Check if any scores were read
    if (i == 0) {
        printf("No scores to display\n");
        TTF_CloseFont(font);
        return;
    }

    // Sort the scores in descending order
    qsort(scores, i, sizeof(Score), compare_scores);

    // Display the top 3 scores on screen
    for (int j = 0; j < 3 && j < i; j++) {
        snprintf(scoreStr, sizeof(scoreStr), "%d. %s: %d", j + 1, scores[j].name, scores[j].score);
        renderText(screen, font, textColor, scoreStr, (screen->w - strlen(scoreStr) * 10) / 2, 50 + j * 50);
    }

    SDL_Flip(screen);
    SDL_Delay(1500);
    TTF_CloseFont(font);
}

void enterPlayerName(char playerName[], SDL_Surface *screen) {
    TTF_Font *font = TTF_OpenFont("arial.ttf", 28);
    if (font == NULL) {
        printf("Error opening font: %s\n", TTF_GetError());
        return;
    }
    SDL_Color textColor = {255, 255, 255}; // White color
    SDL_Event event;
    int loop = 1;

    while (loop) {
        SDL_FillRect(screen, NULL, 0x000000); // Fill the screen with black
        renderText(screen, font, textColor, "Enter Your Name: ", 10, 10);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                case SDL_KEYDOWN:
                    if (strlen(playerName) < 20) {
                        // Append printable characters to the name
                        if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) {
                            playerName[strlen(playerName)] = (char)event.key.keysym.sym;
                            playerName[strlen(playerName) + 1] = '\0';
                        }
                        // Append space character
                        if (event.key.keysym.sym == SDLK_SPACE) {
                            playerName[strlen(playerName)] = ' ';
                            playerName[strlen(playerName) + 1] = '\0';
                        }
                    }
                    // Handle backspace
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(playerName) > 0) {
                        playerName[strlen(playerName) - 1] = '\0';
                    }
                    // Handle enter key to finish input
                    if (event.key.keysym.sym == SDLK_RETURN && strlen(playerName) > 0) {
                        loop = 0;
                    }
                    break;
            }
        }
        renderText(screen, font, textColor, playerName, 200, 50);
        SDL_Flip(screen);
    }
    TTF_CloseFont(font);
}
