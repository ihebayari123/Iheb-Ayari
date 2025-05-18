#ifndef TEXT_H
#define TEXT_H


#include "menu.h" 

void initText(struct GameData* gameData);
void renderText(struct GameData* gameData, const char* text, int x, int y, int fontSize, SDL_Color color);

void renderText2(struct GameData* gameData, const char* text, int x, int y, int fontSize, SDL_Color color);
void cleanupText(struct GameData* gameData);

#endif
