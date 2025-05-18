#include<stdio.h>
#include"SDL/SDL.h"
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include "minimap.h"


void init_minimap(Minimap* m) {
    
    m->mini_image = IMG_Load("minimap img/mini.png");
    m->mini_position.x = 400; 
    m->mini_position.y = 0;
    m->mini_position.w = 180;
    m->mini_position.h = 90;
    
 
    m->player_image = IMG_Load("minimap img/persop.png");
  
    m->player_image2 = IMG_Load("minimap img/persop2.png");
    m->player_position.x = 250;
    m->player_position.y = 200;
    m->minimap_player_position.x = m->mini_position.x + m->player_position.x; 
    m->minimap_player_position.y = m->mini_position.y + m->player_position.y;
   
   m->masque = SDL_LoadBMP("minimap img/bg3.bmp");
}

void MAJMinimap(SDL_Rect posJoueur, Minimap *m, SDL_Rect camera, int redimensionnement, int mapWidth, int mapHeight)
{
    // Calculate the absolute position of the player in the game world
    SDL_Rect posJoueurABS;
    posJoueurABS.x = posJoueur.x + camera.x;
    posJoueurABS.y = posJoueur.y + posJoueur.y;

    // Calculate the position of the player on the minimap
    m->minimap_player_position.x = (posJoueurABS.x * m->mini_position.w / mapWidth) + m->mini_position.x;
    m->minimap_player_position.y = (posJoueurABS.y * m->mini_position.h / mapHeight) + m->mini_position.y;
}




void afficher_minimap(Minimap m, SDL_Surface* screen,int choix) {
    
    SDL_BlitSurface(m.mini_image, NULL, screen, &m.mini_position);
    if (choix ==1)
    SDL_BlitSurface(m.player_image, NULL, screen, &m.minimap_player_position); 
    else if(choix ==2)
    SDL_BlitSurface(m.player_image2, NULL, screen, &m.minimap_player_position); 
    
}
void liberer_minimap(Minimap* m) {
    
    SDL_FreeSurface(m->mini_image);
    SDL_FreeSurface(m->player_image);
}

SDL_Color GetPixel(SDL_Surface* pSurface, int x, int y) {
    SDL_Color color;
    Uint32 col = 0;
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(&col, pPosition, pSurface->format->BytesPerPixel);
    SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);
    return color;
}  


int collisionPP(SDL_Rect player_pos, SDL_Surface* background) {
   
    int posX[8] = {player_pos.x, player_pos.x + player_pos.w / 2, player_pos.x + player_pos.w, player_pos.x, player_pos.x, player_pos.x + player_pos.w / 2, player_pos.x + player_pos.w, player_pos.x + player_pos.w};
    int posY[8] = {player_pos.y, player_pos.y, player_pos.y, player_pos.y + player_pos.h / 2, player_pos.y + player_pos.h, player_pos.y + player_pos.h, player_pos.y + player_pos.h, player_pos.y + player_pos.h / 2};

    
    for (int i = 0; i < 8; i++) {
        SDL_Color pixelColor = GetPixel(background, posX[i], posY[i]);
        if (pixelColor.r == 255 && pixelColor.g == 255 && pixelColor.b == 255) {
            
            return 1;
        }
    }

    
    return 0;
}



void affichertemps(int temps)
{
    SDL_Color color_temp = {0, 0, 0};
    TTF_Font* police_time = NULL;
    police_time = TTF_OpenFont("minimap img/04B_08__.TTF",24);
    char temp[100];
    int heures = temps / 3600;
    int minutes = (temps % 3600) / 60;
    int secondes = temps % 60;
    sprintf(temp, "%02d:%02d:%02d", heures, minutes, secondes);
    SDL_Surface* temps_surface = TTF_RenderText_Solid(police_time, temp, color_temp);
    SDL_Rect pos_temp;
    pos_temp.x = 0;
    pos_temp.y = 0;
    SDL_BlitSurface(temps_surface, NULL, SDL_GetVideoSurface(), &pos_temp);
    TTF_CloseFont(police_time);
    SDL_FreeSurface(temps_surface);
}  


void affichertempsen(Uint32 startTime)
{
    SDL_Color color_temp = {0, 0, 0};
    TTF_Font* police_time = NULL;
    police_time = TTF_OpenFont("minimap img/04B_08__.TTF",23);
    char temp[100];
    int temps = 30000 - (SDL_GetTicks() - startTime); 
    if(temps<0)
    {
    temps=0;}
    int secondes = temps / 1000;
    sprintf(temp, "%02d:%02d", 0, secondes); 

    
    SDL_Surface* temps_surface = NULL;
    temps_surface = TTF_RenderText_Solid(police_time, temp, color_temp);

    
    SDL_Rect pos_temp;
    pos_temp.x = 20;
    pos_temp.y = 20;

    
    SDL_BlitSurface(temps_surface, NULL, SDL_GetVideoSurface(), &pos_temp);
    
    
    TTF_CloseFont(police_time);
    SDL_FreeSurface(temps_surface);
}




void animerMinimap(Minimap* m) {
    SDL_Surface* surface = m->mini_image;

    
    Uint32 color;
    static int frame = 0;
    frame++;
    if (frame % 50 < 25) {
        color = SDL_MapRGB(surface->format, 255, 0, 0); 
    } else {
        color = SDL_MapRGB(surface->format, 0, 255, 0); 
    }

    
    SDL_Rect point_rect = { 280,55 , 15, 15  };

    
    SDL_FillRect(surface, &point_rect, color);

   
    SDL_UpdateRect(surface, 0, 0, 0, 0);
}


void animerMinimape(Minimap* m) {
    SDL_Surface* surface = m->mini_image;

    
    Uint32 color;
    static int frame = 0;
    frame++;
    if (frame % 50 < 25) {
        color = SDL_MapRGB(surface->format, 0, 140, 255); 
    } else {
        color = SDL_MapRGB(surface->format, 255, 140, 0); 
    }

    
    SDL_Rect point_rect = { 180,90 , 15, 15  };

    
    SDL_FillRect(surface, &point_rect, color);

   
    SDL_UpdateRect(surface, 0, 0, 0, 0);
}


void animerMinimape1(Minimap* m) {
    SDL_Surface* surface = m->mini_image;

    
    Uint32 color;
    static int frame = 0;
    frame++;
    if (frame % 50 < 25) {
        color = SDL_MapRGB(surface->format, 0, 140, 255); 
    } else {
        color = SDL_MapRGB(surface->format, 255, 140, 0); 
    }

    
    SDL_Rect point_rect = { 145,55 , 15, 15  };

    
    SDL_FillRect(surface, &point_rect, color);

   
    SDL_UpdateRect(surface, 0, 0, 0, 0);
}



