#define MAX_NAME_LENGTH 50
#ifndef MENU_H
#define MENU_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "perso.h"
enum MenuState {
    MAIN_MENU,
    OPTIONS_MENU,
    PLAY_MENU,
    CREDITS_MENU,
    
};


enum Playmenu {
    AFFBG,
    PLAYER1,
    typen,
    INGAME,
   
};





struct GameData {
    char playerName[50];
    int quitter;
    SDL_Surface* ecran;
    SDL_Surface* images[2];
    
    SDL_Surface* animation[2];
    SDL_Surface* animation2[2];
    
    SDL_Surface* cigar[9];
    
    SDL_Surface* s[11];
    
    SDL_Surface* buttons[4];
    SDL_Surface* buttons2[4];
    
    SDL_Surface* xbutton;
    SDL_Surface* xbutton2;
    
    SDL_Surface* option[6];
    SDL_Surface* option2[3];
    
    SDL_Surface* menuplay;
    
    SDL_Surface* character1;
    SDL_Surface* character2;
    
    SDL_Surface* floue;
    
    SDL_Surface* omnistrike[3];
    
    SDL_Surface* rise[3];
    
    SDL_Surface* select1player[2];
    SDL_Surface* select2player[2];
    
    SDL_Surface* videos;
    
    SDL_Surface* bgselect;
    SDL_Surface* textcharacter;
    
    SDL_Surface* selectcharacter[2];
    
    SDL_Surface* MaxHarper[2];
    
    SDL_Surface* selectch1design[3];
    SDL_Surface* selectch1design2[3];
    
    SDL_Surface* PRESS[3];
    
    SDL_Surface* PRESSP;
    SDL_Surface* typeName;
    
    
    
    Mix_Music* musique;
    Mix_Chunk* sonb;
    
    TTF_Font* font;
    
    int screenWidth;
    int screenHeight;
    int currentImage;
    int specificBoxClicked;
    
    int showButtons;
    int showButtons10;
    
    int showx;
    int showoption;
    
    int soundVolume;
    
    int play;
    int setting;
    int exit;
    
    int x;
    int plus2;
    int moin2;
    int fullscreen2;
    int sound;
    int b;
    int pleinecran;
     
     int pe;
    
    int position2;
     
    int isFullscreen;
    int cigara;
    
    int credit;
    int flou;
    
    
    int showxflou;
    
    
    int showmenu;
    
    int showselect1player;
    int showselect2player;
    
    int bgs;
    int chaselect1;
    int chaselect2;
    
    
    
    
    
    //position machine
    SDL_Rect machineposition;
    //position top
    SDL_Rect positiontop[2];
    
    
    //postion de bg option
    SDL_Rect postionoption;
    //position du sound
    SDL_Rect positionsound;
    //position de button +
    SDL_Rect dstRectplus;
    //position de button -
    SDL_Rect dstRectmoin;
    
    //position de button plein ecran
    SDL_Rect dstRectpp;
    
    
    
    SDL_Rect dstRectpe;
    //position de X button
    SDL_Rect dstRectf;
    SDL_Rect dstRectf2;
    //position des bouttons play option credit exit
    SDL_Rect dstRect;
    SDL_Rect dstRectp;
    SDL_Rect dstRect2;
    SDL_Rect dstRectc;
    //positon bg
    SDL_Rect positionImage;
    SDL_Rect videopos;
    //image flou
    SDL_Rect flouRect;
    
    //position cigar
    SDL_Rect cigarpos;
    
    //position menuplay
    SDL_Rect menuplaypos;
    
    //position de text omnistrike
    SDL_Rect omnistrikepos;
    //potisiotn rise of the resistance
    SDL_Rect risepos;
    
    //1player
    SDL_Rect select1playerpos;
    SDL_Rect select1playerpos2;
    //2player
    SDL_Rect select2playerpos;
    SDL_Rect select2playerpos2;
    
    
    
    //bg select player 1
    SDL_Rect bgselectpos;
    
    //textcharacter select position
    SDL_Rect textcharacterpos;
    
    
    //select character pos
    SDL_Rect selectcharacterpos;
    SDL_Rect selectcharacterpos2;
    //MAX HARPER CHARACTER SELECT pos
    SDL_Rect MaxHarperpos[2];
    
    //press position
    SDL_Rect PRESSpos;
    SDL_Rect PRESSPpos;
    
    
    //type name position image
    SDL_Rect typeNamepos;
    
   
    
    
    
    
    
    
    
    
    SDL_Rect c1;
    SDL_Rect c2;
    
    
    
    
    enum MenuState menuState;
    enum Playmenu playmenu;
    
   

};


 struct Perso;




void drawButtons(struct GameData* gameData); 

void initializeGame(struct GameData* gameData);
void loadImages(struct GameData* gameData);
void cleanupGame(struct GameData *gameData);
void handleTypingEvent(struct GameData* gameData, SDL_Event event);
void initText(struct GameData* gameData);
void renderText(struct GameData* gameData, const char* text, int x, int y, int fontSize, SDL_Color color);
void renderText2(struct GameData* gameData, const char* text, int x, int y, int fontSize, SDL_Color color);
void cleanupText(struct GameData* gameData);


#endif


