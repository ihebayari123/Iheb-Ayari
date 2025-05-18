#ifndef SCORE_H
#define SCORE_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

typedef struct {
    char name[30];
    int score;
} Score;

void save_score(Score score);
void show_top_scores(SDL_Surface *screen);
void enterPlayerName(char playerName[], SDL_Surface *screen);
void renderText(SDL_Surface *surface, TTF_Font *font, SDL_Color color, const char *text, int x, int y);

#endif // SCORE_H

