
#ifndef map_H_INCLUDED
#define map_H_INCLUDED

typedef struct 
{	
	SDL_Surface *backg ,*retry;
	SDL_Surface *tic1 ;
	SDL_Surface *tic2; 
	SDL_Rect pos;
	SDL_Rect postexte;
	SDL_Rect tab_pos[9];
	int tab_x_o[9];
	int counter;
     char tabnumbers[20];
	 SDL_Surface *h_line,*v_line,*tilt_left_line,*tilt_right_line,*score_surf;
	 int reset_state;
	 int score;
	 int ms,s,m;
	 SDL_Rect pos_txt;
SDL_Surface *tsurf ,*ts_score,*t_timer,*score_txt;
TTF_Font *police,*p_score,*p_timer;
SDL_Color coul;
SDL_Rect rect_score;
char tixt[20];

}tic;
void initialisertic(tic *t);



void set_position(SDL_Event event,tic *t);
void afficher(tic *t,SDL_Surface *window);
int getRandomNumberFromString(const char* str);
int calcul_coup(tic *t);
int o_win(tic *t);
void retry_ui(tic *t,SDL_Surface *screen,SDL_Event event);
void change_score(tic *t,SDL_Event event);
void init_timer_display_engm(tic *t);
void afficher_timer_display_(SDL_Surface *screen,tic *t);
int initializeSDL() ;
Mix_Music* loadMusic(const char* filename) ;
int playMusic(Mix_Music* music) ;
void cleanup(Mix_Music* music) ;
#endif
