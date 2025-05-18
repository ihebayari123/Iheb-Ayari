#include "text.h"

void initText(struct GameData* gameData) {
    if (TTF_Init() == -1) {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    // Load a font
    gameData->font = TTF_OpenFont("typetext/Minecraft.ttf", 28);
    if (gameData->font == NULL) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        exit(1);
    }
    
    
}

void renderText(struct GameData* gameData, const char* text, int x, int y, int fontSize, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(gameData->font, text, color);
    if (surface == NULL) {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Rect rect = { x, y, 0, 0 };
    SDL_BlitSurface(surface, NULL, gameData->ecran, &rect);

    SDL_FreeSurface(surface);
}

void renderText2(struct GameData* gameData, const char* text, int x, int y, int fontSize, SDL_Color color) {
    // Check if the text is empty
    if (text[0] == '\0') {
        // If the text is empty, do nothing and return
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(gameData->font, text, color);
    if (surface == NULL) {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Rect rect = { x, y, 0, 0 };
    SDL_BlitSurface(surface, NULL, gameData->ecran, &rect);

    SDL_FreeSurface(surface);
}





void cleanupText(struct GameData* gameData) {

    TTF_CloseFont(gameData->font);
    TTF_Quit();
}
