#include<stdio.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "blanc.h"
#include <string.h>
#include <time.h>


int getRandomNumberFromString(const char* str) {
    int length = strlen(str);
    srand(time(NULL));
    int randomIndex = rand() % length;
    int randomNumber = str[randomIndex] - '0'; 
    return randomNumber;
}

void initialisertic(tic *t)
{
	int i;
	strcpy(t->tabnumbers,"");
	t->score=0;
	t->reset_state=0;
	t->counter=0;
	t->backg = IMG_Load("background.jpg");
	t->h_line=IMG_Load("h_line.png");
	t->v_line=IMG_Load("v_line.png");
	t->score_surf=IMG_Load("congrats.jpg");
	t->tilt_right_line=IMG_Load("tilt_right.png");
	t->tilt_left_line=IMG_Load("tilt_left.png");
	t->tic1=IMG_Load("x.png");
        t->tic2=IMG_Load("o.png");
	for(i=0;i<9;i++)
	{	
		t->tab_x_o[i]=0;
	}
	t->pos.x = 20;
	t->pos.y = 20;
	t->tab_pos[0].x=47;
	t->tab_pos[0].y=199;
	t->tab_pos[1].x=300;
	t->tab_pos[1].y=199; 
	t->tab_pos[2].x=536;
	t->tab_pos[2].y=199; 
	t->tab_pos[3].x=47;
	t->tab_pos[3].y=444;   
	t->tab_pos[4].x=300;
	t->tab_pos[4].y=444; 
	t->tab_pos[5].x=536;
	t->tab_pos[5].y=444; 
	t->tab_pos[6].x=47;
	t->tab_pos[6].y=677;  
	t->tab_pos[7].x=300;
	t->tab_pos[7].y=677; 
	t->tab_pos[8].x=536;
	t->tab_pos[8].y=677; 
}




//////////////////////////
void set_position(SDL_Event event,tic *t)
{

	
	if((event.motion.x <= 277 && event.motion.x >= 47 && event.motion.y <= 423 && event.motion.y >= 199))
	{   
             if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[0]!=1)
	t->tab_x_o[0]=2;}
		
	}
	if((event.motion.x <= 518 && event.motion.x >= 208 && event.motion.y <= 423 && event.motion.y >= 199))
	{
		if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[1]!=1)
		  t->tab_x_o[1]=2; 
		}
	}
	if((event.motion.x <= 763 && event.motion.x >= 536 && event.motion.y <= 423 && event.motion.y >= 199))
	{
		if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[2]!=1)
		  t->tab_x_o[2]=2;}
		
	}
	if((event.motion.x <= 277 && event.motion.x >= 47 && event.motion.y <= 659 && event.motion.y >= 444))
	{   
		if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[3]!=1)
	t->tab_x_o[3]=2;}
		
	}
	if((event.motion.x <= 424 && event.motion.x >= 208 && event.motion.y <= 659 && event.motion.y >= 444))
	{   
	if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[4]!=1)
	t->tab_x_o[4]=2;}
		
	}
	if((event.motion.x <= 763 && event.motion.x >= 536 && event.motion.y <= 659 && event.motion.y >= 444))
	{   
		if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[5]!=1)
	t->tab_x_o[5]=2;}
		
	}
	if((event.motion.x <= 277 && event.motion.x >= 47 && event.motion.y <= 961 && event.motion.y >= 677))
	{   
		if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[6]!=1)
	t->tab_x_o[6]=2;}
		
	}
	if((event.motion.x <= 518 && event.motion.x >= 296 && event.motion.y <= 961 && event.motion.y >= 677))
	{   
		if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[7]!=1)
	t->tab_x_o[7]=2;}
		
	}
	if((event.motion.x <= 763 && event.motion.x >= 536 && event.motion.y <= 961 && event.motion.y >= 677))
	{   
	if (event.button.button == SDL_BUTTON_LEFT){
			if(t->tab_x_o[8]!=1)
	t->tab_x_o[8]=2;}
		
	
		}
	strcpy(t->tabnumbers,"");
    for(int i =0;i<8;i++)
	{
		if(t->tab_x_o[i]==0)
		    sprintf(t->tabnumbers + strlen(t->tabnumbers), "%d", i);
			
	
	}
    t->tab_x_o[getRandomNumberFromString(t->tabnumbers)]=1;
   
}
int o_win(tic *t)
{
	int value;
	if(t->tab_x_o[0]==1 && t->tab_x_o[1]==1 && t->tab_x_o[2]==1){
		
               value=1;}
               if(t->tab_x_o[0]==1){
		if(t->tab_x_o[1]==1){
		if(t->tab_x_o[2]==1){
               value=1;}
		}
		}
		
		if(t->tab_x_o[3]==1){
		if(t->tab_x_o[4]==1){
		if(t->tab_x_o[5]==1){
               value=2;}
		}
		}
		if(t->tab_x_o[6]==1){
		if(t->tab_x_o[7]==1){
		if(t->tab_x_o[8]==1){
               value=3;}
		}
		}
		if(t->tab_x_o[0]==1){
		if(t->tab_x_o[3]==1){
		if(t->tab_x_o[6]==1){
               value=4;}
		}
		}
		if(t->tab_x_o[1]==1){
		if(t->tab_x_o[4]==1){
		if(t->tab_x_o[7]==1){
               value=5;}
		}
		}
		if(t->tab_x_o[2]==1){
		if(t->tab_x_o[5]==1){
		if(t->tab_x_o[8]==1){
              value=6;}
		}
		}
		if(t->tab_x_o[0]==1){
		if(t->tab_x_o[4]==1){
		if(t->tab_x_o[8]==1){
               value=7;}
		}
		}
		if(t->tab_x_o[2]==1){
		if(t->tab_x_o[4]==1){
		if(t->tab_x_o[6]==1){
			value=8;
               }
		}
		}
		return value;
}
int calcul_coup(tic *t){
int value=0;

		if(t->tab_x_o[0]==2 && t->tab_x_o[1]==2 && t->tab_x_o[2]==2){
		
               value=1;}
               if(t->tab_x_o[0]==2){
		if(t->tab_x_o[1]==2){
		if(t->tab_x_o[2]==2){
               value=1;}
		}
		}
		
		if(t->tab_x_o[3]==2){
		if(t->tab_x_o[4]==2){
		if(t->tab_x_o[5]==2){
               value=2;}
		}
		}
		if(t->tab_x_o[6]==2){
		if(t->tab_x_o[7]==2){
		if(t->tab_x_o[8]==2){
               value=3;}
		}
		}
		if(t->tab_x_o[0]==2){
		if(t->tab_x_o[3]==2){
		if(t->tab_x_o[6]==2){
               value=4;}
		}
		}
		if(t->tab_x_o[1]==2){
		if(t->tab_x_o[4]==2){
		if(t->tab_x_o[7]==2){
               value=5;}
		}
		}
		if(t->tab_x_o[2]==2){
		if(t->tab_x_o[5]==2){
		if(t->tab_x_o[8]==2){
              value=6;}
		}
		}
		if(t->tab_x_o[0]==2){
		if(t->tab_x_o[4]==2){
		if(t->tab_x_o[8]==2){
               value=7;}
		}
		}
		if(t->tab_x_o[2]==2){
		if(t->tab_x_o[4]==2){
		if(t->tab_x_o[6]==2){
			value=8;
               }
		}
		}
		return value;

}

void afficher(tic *t,SDL_Surface *screen)
{
	int value=calcul_coup(t);
	SDL_Rect backg_pos;
	backg_pos.x = 0;
	backg_pos.y = 0;
	SDL_Rect rect;
        SDL_BlitSurface(t->backg,NULL,screen,&backg_pos);
		for(int i=0;i<9;i++)
		{
			if(t->tab_x_o[i]==2)
              SDL_BlitSurface(t->tic1,NULL,screen,&t->tab_pos[i]);
			if(t->tab_x_o[i]==1)
              SDL_BlitSurface(t->tic2,NULL,screen,&t->tab_pos[i]);	
		}
		if(value==1){
rect.x=30;
rect.y=280;
 SDL_BlitSurface(t->h_line,NULL,screen,&rect);
		}
		if(value==2){
rect.x=30;
rect.y=530;
 SDL_BlitSurface(t->h_line,NULL,screen,&rect);
		}
 if(value==3){
rect.x=30;
rect.y=760;
 SDL_BlitSurface(t->h_line,NULL,screen,&rect);
		}
		if(value==4){
rect.x=110;
rect.y=150;
 SDL_BlitSurface(t->v_line,NULL,screen,&rect);
		}
		if(value==5){
rect.x=350;
rect.y=150;
 SDL_BlitSurface(t->v_line,NULL,screen,&rect);
		}
		if(value==6){
rect.x=600;
rect.y=150;
 SDL_BlitSurface(t->v_line,NULL,screen,&rect);
		}
		if(value==7){
rect.x=50;
rect.y=200;
 SDL_BlitSurface(t->tilt_left_line,NULL,screen,&rect);
		}
 if(value==8){
rect.x=50;
rect.y=200;
 SDL_BlitSurface(t->tilt_right_line,NULL,screen,&rect);
		}
		if(t->score>1500)
		SDL_BlitSurface(t->score_surf,NULL,screen,NULL);
		
}
void retry_ui(tic *t, SDL_Surface *screen,SDL_Event event)
{

	t->retry=IMG_Load("lost.jpg");
	int value=o_win(t);
	if(value>0 && value<9)
		t->reset_state=1;
		if((event.motion.x <= 640 && event.motion.x >= 158 && event.motion.y <= 659 && event.motion.y >= 562))
	{   
	if (event.button.button == SDL_BUTTON_LEFT){
        SDL_Quit();
	}
	}
	if((event.motion.x <= 640 && event.motion.x >= 158 && event.motion.y <= 480 && event.motion.y >= 370))
	{   
	if (event.button.button == SDL_BUTTON_LEFT){
		t->score=0;
        for(int i=0;i<9;i++)
	{
		t->tab_x_o[i]=0;
	}
	t->reset_state=0;
	}
	}
	if(t->reset_state==1)
	SDL_BlitSurface(t->retry,NULL,screen,NULL);
}

void change_score(tic *t,SDL_Event event)
{
	
	
	
	int value=calcul_coup(t);
	if(value>0 && value<9){
		if((event.motion.x <= 752 && event.motion.x >= 658 && event.motion.y <= 147 && event.motion.y >= 53))
	{   
	if (event.button.button == SDL_BUTTON_LEFT){
		t->ms=10;
        t->s=0;
        t->m=0;
	 for(int i=0;i<9;i++)
	{
		t->tab_x_o[i]=0;
	}
	t->score+=500;
	
	}
	}
	}
     
}

void init_timer_display_engm(tic *t){
TTF_Init();
t->p_timer=TTF_OpenFont("angelina.ttf",70);
t->pos_txt.x=0;
t->pos_txt.y=20;
t->coul.r=0;
t->coul.g=180;
t->coul.b=180;
t->ms=10;
t->s=0;
t->m=0;
t->rect_score.x=400;
	t->rect_score.y=30;
}

void afficher_timer_display_(SDL_Surface *screen,tic *t){
	if(t->score<1500){
char timer[4];  
char txt[20];  
        t->ms--;
        if(t->ms<0)
        {
          t->ms=10;
          t->s--;
          if(t->s<0)
          {
            t->s=60;
            if(t->s<=1)
            t->s=0;
            if(t->m<0)
            t->m=0;
          }
         
        }
       
         if(t->s==1)
          t->reset_state=1;
        sprintf(timer,"%d:%d",t->m,t->s);
t->t_timer=TTF_RenderText_Blended(t->p_timer,timer,t->coul);
SDL_BlitSurface(t->t_timer,NULL,screen,&t->pos_txt);
sprintf(txt,"%s:%d","score",t->score);
t->score_txt=TTF_RenderText_Blended(t->p_timer,txt,t->coul);
SDL_BlitSurface(t->score_txt,NULL,screen,&t->rect_score);
	}
}

int initializeSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return 0;
    }
    return 1;
}


Mix_Music* loadMusic(const char* filename) {
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Mix_LoadMUS: %s\n", Mix_GetError());
    }
     return music;
}
int playMusic(Mix_Music* music) {
    if (Mix_PlayMusic(music, -1) == -1) {
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
        return 0;
    }
    return 1;
}

void cleanup(Mix_Music* music) {
    Mix_FreeMusic(music);
    Mix_CloseAudio();
   
}
