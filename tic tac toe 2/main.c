
#include<stdio.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "blanc.h"
#include <string.h>

int main()
{	
	int continuer=1;
	SDL_Surface *screen,*back;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	screen=SDL_SetVideoMode(805,1080,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	tic t;
	initialisertic(&t);
	SDL_Event event;
	init_timer_display_engm(&t);

    initializeSDL();
	Mix_Music* music = loadMusic("music.mp3");
	 playMusic(music) ;
	while (continuer)
	{	
	 int x, y;
    SDL_GetMouseState(&x, &y);
	printf("x:%d\ny:%d\n",x,y);
	
	 
	while (SDL_PollEvent( &event)){
      switch (event.type) {
        case SDL_QUIT:
          continuer = 0;
          break;
        case SDL_MOUSEMOTION:      
          break;
        
          case SDL_MOUSEBUTTONDOWN:
		  set_position(event,&t);
		   change_score(&t,event);
          

      break;
      }
	  if (event.key.keysym.sym == SDLK_ESCAPE)
          continuer=0;
    }
	afficher(&t,screen);
	afficher_timer_display_(screen,&t);
	retry_ui(&t,screen,event);
	SDL_Flip(screen);
	} 
	cleanup(music);
	TTF_Quit();
	SDL_Quit();

	return 1;
}
