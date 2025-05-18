#include "menu.h"
#include "background.h"
#include "perso.h"
#include "enigme.h"
#include "minimap.h"
#include <string.h>
#include <SDL/SDL.h>
#include <time.h>
#include "xo.h"
bool keys[SDLK_LAST] = { false };
int main(int argc, char** argv) {



SDL_Surface* introImage = IMG_Load("in game image/intro_image.png");
SDL_Surface* introImage2 = IMG_Load("in game image/intro2_image.png");
SDL_Rect introImagePosition;
SDL_Surface* talk = IMG_Load("in game image/talk.png");
SDL_Surface* talk2 = IMG_Load("in game image/talk2.png");
SDL_Surface* talk3 = IMG_Load("in game image/talk3.png");

SDL_Surface* zoom = IMG_Load("in game image/zoom.png");
SDL_Surface* zoom2 = IMG_Load("in game image/zoom2.png");
SDL_Surface* zoom3 = IMG_Load("in game image/zoom3.png");

SDL_Surface* conv = IMG_Load("in game image/conversation.png");
SDL_Surface* conv2 = IMG_Load("in game image/conversation2.png");
SDL_Surface* conv3 = IMG_Load("in game image/conversation3.png");
SDL_Surface* conv4 = IMG_Load("in game image/conversation4.png");

SDL_Surface* pressj = IMG_Load("in game image/pressj.png");

SDL_Surface* hand = IMG_Load("in game image/hand.png");

SDL_Rect talkpos;
SDL_Rect zoompos;
SDL_Rect pressjpos;
SDL_Rect handpos;

introImagePosition.x =200;
introImagePosition.y=100;

talkpos.x=950;
talkpos.y=360;

zoompos.x=130;
zoompos.y=300;

pressjpos.x=200;
pressjpos.y=385;

handpos.x=0;
handpos.y=30;


bool displayIntroImage = true;
bool displayIntroImage2 = true;
bool displaytalk = false;
bool talkDisplayed = false;

int displayStartTime = 0;
bool zoomVisible = false;
bool zoomVisible2 = false; // Variable to track if zoom is currently visible
time_t zoomStartTime = 0; // Variable to store the start time when zoom is shown
time_t zoomStartTime2 = 0;

    struct GameData gameData;
    
    Personne perso;
    Background b,b2;
    Back b3;
    enigme e,e1,e2,e3,e4;
    tic t;
    Minimap m;
    int direction;
    int pas=10;
    int flag=0;
    int ctrl_pressed;
    int test = 0;
    bool inGame = false;
    int joueur; 
    int a,bb;
    int coup;
    int showtalk =0;
    int showtalk2 =0;
    int zoomshow=0;
    int zoom2visible=0;
    int zoom3visible=0;
    int showconv=0;
    int showconv1=0;
    int showconv2=0;
    int showconv3=0;
    int showpressj=0;
    int showhand =0;
   perso.numperso = 0;
    Uint32 start_time = SDL_GetTicks();

   initializeGame(&gameData);
   gameData.playerName[0]='\0';
   printf("%s\n",gameData.playerName);
    loadImages(&gameData);
    initBack(&b);
    initBack(&b2);
    initBackPartage(&b2);
    initanim(&b3);
    initPerso(&perso);
    initenigme(&e);
    init_minimap(&m);
    initialisation(&t);
     b.camera.x = 0;
    b.camera.y = 0;
   b.camera.w = gameData.ecran->w;
    b.camera.h = gameData.ecran->h;
    
    int leftPressed = 0;
    int rightPressed = 0;
    bool multiplayerMode = false;

    InitEnigme(&e1, "inigme img/niveau1.txt");
    InitEnigme(&e2, "inigme img/niveau2.txt");
    InitEnigme(&e3, "inigme img/niveau3.txt");
    InitEnigme(&e4, "inigme img/niveau4.txt");
    
    while (gameData.quitter) {
      
        SDL_Event event;
        //talk position with camera 
       SDL_Rect destRect = { talkpos.x - b.camera.x, talkpos.y - b.camera.y, 0, 0 };
       SDL_Rect destRect2 = { zoompos.x - b.camera.x, zoompos.y - b.camera.y, 0, 0 };
      while (SDL_PollEvent(&event)) {
      
    switch (event.type) {
        case SDL_QUIT:
             gameData.sound=1;
            gameData.quitter = 0;
            break;
       
        case SDL_KEYDOWN:
         
         
             keys[event.key.keysym.sym] = true;
                
            if (gameData.menuState == OPTIONS_MENU){         
            if (event.key.keysym.sym == SDLK_g) {
            gameData.sound=1;
               if (gameData.ecran->flags & SDL_FULLSCREEN) {
                // Switch to windowed mode
                 gameData.ecran = SDL_SetVideoMode(1120, 800, 32, SDL_HWSURFACE | SDL_RESIZABLE);
                } else {
               // Switch to fullscreen mode
               gameData.ecran = SDL_SetVideoMode(gameData.screenWidth, gameData.screenHeight, 32, SDL_HWSURFACE | SDL_FULLSCREEN);
                  }
               }
               }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                
                           gameData.quitter = 0; 
                           gameData.sound=1;
                        
            }
             if (gameData.menuState == MAIN_MENU){
            if (event.key.keysym.sym == SDLK_j) {
               
                       
                        
                         gameData.showx = 1;
                         
                         gameData.sound=1;
                         
                         gameData.menuState = PLAY_MENU;
                        
            }
            }
            if (gameData.menuState == MAIN_MENU){
            if (event.key.keysym.sym == SDLK_o) {
               
                         
                         
                          gameData.showx = 1;
                         
                          gameData.sound=1;
                          gameData.menuState = OPTIONS_MENU;
                        
            }
            }
           if (gameData.playmenu == PLAYER1){
           if (gameData.chaselect2 || gameData.chaselect1){
           if (event.key.keysym.sym == SDLK_p) {
                       
                          gameData.sound=1;
                          gameData.playmenu = typen;
                          
                }        
            }
            }
            
            if (gameData.menuState == OPTIONS_MENU){
            if (event.key.keysym.sym == SDLK_u) {
                 gameData.soundVolume += 10;
                  gameData.sound=1;  
                 if (gameData.soundVolume > 100) {
                     gameData.soundVolume = 100;
                    
                 }
                 Mix_VolumeMusic(gameData.soundVolume);
            }
            if (event.key.keysym.sym == SDLK_i) {
                gameData.soundVolume -= 10; 
                 gameData.sound=1; 
                           if (gameData.soundVolume < 0) {
                               gameData.soundVolume = 0;
                               gameData.sound=1; 
                           }
                           Mix_VolumeMusic(gameData.soundVolume);
            }
            }
            
           if (gameData.playmenu == typen){
            handleTypingEvent(&gameData, event);
            }
            
            if (gameData.playmenu == typen){
            if (event.key.keysym.sym == SDLK_RETURN) {
            gameData.playmenu = INGAME;
            }
            }
            if( gameData.playmenu == INGAME){
                     if (event.key.keysym.sym == SDLK_RIGHT) {
                      if (ctrl_pressed) {
                            perso.speed = 10;
                            perso.move = 1;
                
                 
        
                     if (perso.posScreen.x > 1120) {
            perso.posScreen.x = 1120;
                     direction=0;
            
                        } 
                        }else {
                            perso.speed = 5;
                            perso.move = 1;
                
                
                            }
              
              perso.direction =1;
              if (perso.numperso == 1)
              perso.current_sprite =perso.run;
              if (perso.numperso == 2)
              perso.current_sprite2 =perso.run2;
                     }  
                     
                     if (event.key.keysym.sym == SDLK_d) {
                     direction=0;
                     
                     
                     }
                     
                     if (event.key.keysym.sym == SDLK_LEFT) {
                     if (ctrl_pressed) {
                            perso.speed = 10; 
                            perso.move = 1;

                     if (perso.posScreen.x < 0) {
            perso.posScreen.x = 0;
                     }
                     
                        } else {
                            perso.speed = 5;
                            perso.move = 1;
      
                 }
              
              perso.direction =-1;
              if (perso.numperso == 1)
              perso.current_sprite =perso.runleft;
              if (perso.numperso == 2)
              perso.current_sprite2 =perso.runleft2;
                     } 
                     
                     if (event.key.keysym.sym == SDLK_q) {
                     direction=-1;
                     
                     }
                     
                      if (event.key.keysym.sym == SDLK_SPACE) {
   
                            flag = 1;
                           
                     }
                        
            
           if (event.key.keysym.sym == SDLK_m) {
                        multiplayerMode = !multiplayerMode;
                        }
           if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                 ctrl_pressed = 1;
                 }
                 
                 if (event.key.keysym.sym == SDLK_p) {
                     displayIntroImage = false;
                     displayIntroImage2 = false;
                     }
            }
           
            if (event.key.keysym.sym == SDLK_t && showconv == 1) {
            showconv =0;
            showconv1 =1;
            }
            
            
            else if (event.key.keysym.sym == SDLK_t && showconv1 ==1){
            showconv1 =0;
            showconv2 =1;
            }
            
            else if (event.key.keysym.sym == SDLK_t && showconv2 ==1){
            showconv2 =0;
            showconv3 =1;
            }
            else if (event.key.keysym.sym == SDLK_t && showconv3 ==1){
            showconv3 =0;
            showpressj=1;
           
            }
            
            if (event.key.keysym.sym == SDLK_j && showpressj == 1) {
            showpressj =0;
            showhand=1;
            }
            
            else if (event.key.keysym.sym == SDLK_j && showhand == 1) {
            
            showhand=0;
            }
            
            break;
            case SDL_KEYUP:
            keys[event.key.keysym.sym] = false;
            	if( gameData.playmenu == INGAME){
                     if (event.key.keysym.sym == SDLK_RIGHT) { 
                     
                     perso.move = 0;
                     perso.direction = 0;
                     if (perso.numperso == 1)
                     perso.current_sprite =perso.idle;
                     if (perso.numperso == 2)
                     perso.current_sprite2 =perso.idle2;
                     }  
                     
                     if (event.key.keysym.sym == SDLK_LEFT) {
                     
                     perso.move = 0;
                     perso.direction = 0;
                     if (perso.numperso == 1)
                     perso.current_sprite =perso.idle;
                     if (perso.numperso == 2)
                     perso.current_sprite2 =perso.idle2;
                     } 
                     if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                 ctrl_pressed = 0;
                 }
                  if (event.key.keysym.sym == SDLK_SPACE) {
             
                     
            
                            flag = 0;
                     }
                     
                     
                     
            }
            
        
         
            
         case SDL_MOUSEBUTTONDOWN:
                if (event.button.button  == SDL_BUTTON_LEFT) {
                  
                   if (gameData.playmenu == MAIN_MENU){
                    if (event.button.x >= gameData.dstRect.x && event.button.x <= gameData.dstRect.x + gameData.dstRect.w &&
                        event.button.y >= gameData.dstRect.y && event.button.y <= gameData.dstRect.y + gameData.dstRect.h) {
                    
                       
                         gameData.showx = 1;
                         
                         gameData.sound=1;
                         
                         gameData.menuState = PLAY_MENU;
                         gameData.playmenu = AFFBG;
                        }
                    
                    }
                    if (gameData.menuState == MAIN_MENU){
                    if (event.button.x >= gameData.dstRectp.x && event.button.x <= gameData.dstRectp.x + gameData.dstRectp.w &&
                        event.button.y >= gameData.dstRectp.y && event.button.y <= gameData.dstRectp.y + gameData.dstRectp.h) {
                        
                         
                         
                          gameData.showx = 1;
                          gameData.sound=1;
                          gameData.menuState = OPTIONS_MENU;
                        
                    }
                    }
                    
                
                    
                    if (event.button.x >= gameData.dstRect2.x && event.button.x <= gameData.dstRect2.x + gameData.dstRect2.w &&
                        event.button.y >= gameData.dstRect2.y && event.button.y <= gameData.dstRect2.y + gameData.dstRect2.h) {
                        
                           gameData.quitter = 0;
                           gameData.sound=1; 
                        
                    }
                    if (event.button.x >= gameData.dstRectf.x && event.button.x <= gameData.dstRectf.x + gameData.dstRectf.w &&
                        event.button.y >= gameData.dstRectf.y && event.button.y <= gameData.dstRectf.y + gameData.dstRectf.h ||
                       event.button.x >= gameData.dstRectf2.x && event.button.x <= gameData.dstRectf2.x + gameData.dstRectf2.w &&
                        event.button.y >= gameData.dstRectf2.y && event.button.y <= gameData.dstRectf2.y + gameData.dstRectf2.h ) {
                       
                        
                         
                         gameData.sound=1;
                         if (gameData.menuState == OPTIONS_MENU)
                         {
                         gameData.menuState = MAIN_MENU;
                         gameData.showx = 0;
                         }
                         if (gameData.playmenu == AFFBG)
                         {
                         gameData.menuState = MAIN_MENU;
                         gameData.showx = 0;
                         }
                         if (gameData.playmenu == PLAYER1)
                         {
                         gameData.playmenu = AFFBG;
                         gameData.showx = 1;
                         }
                         
                         if (gameData.playmenu == typen)
                         {
                         gameData.playmenu = PLAYER1;
                         gameData.showx = 1;
                         }
                        
                    }
                    if (event.button.x >= gameData.dstRectplus.x && event.button.x <= gameData.dstRectplus.x + gameData.dstRectplus.w &&
                        event.button.y >= gameData.dstRectplus.y && event.button.y <= gameData.dstRectplus.y + gameData.dstRectplus.h) {
                           gameData.soundVolume += 10;
                           gameData.sound=1; 
                           if (gameData.soundVolume > 100) {
                               gameData.soundVolume = 100; 
                               
                           }
                           Mix_VolumeMusic(gameData.soundVolume);
                        }
                    if (event.button.x >= gameData.dstRectmoin.x && event.button.x <= gameData.dstRectmoin.x + gameData.dstRectmoin.w &&
                        event.button.y >= gameData.dstRectmoin.y && event.button.y <= gameData.dstRectmoin.y + gameData.dstRectmoin.h) {
                           gameData.soundVolume -= 10;
                           gameData.sound=1; 
                           if (gameData.soundVolume < 0) {
                               gameData.soundVolume = 0; 
                           }
                           Mix_VolumeMusic(gameData.soundVolume);
                        }    
                      
                        
                  if (event.button.x >= gameData.dstRectpp.x && event.button.x <= gameData.dstRectpp.x + gameData.dstRectpp.w &&
    event.button.y >= gameData.dstRectpp.y && event.button.y <= gameData.dstRectpp.y + gameData.dstRectpp.h) {
    gameData.sound = 1;
    if (gameData.ecran != NULL) {
    if (gameData.ecran->flags & SDL_FULLSCREEN) {
        // Switch to windowed mode
        gameData.ecran = SDL_SetVideoMode(1120, 800, 32, SDL_HWSURFACE | SDL_RESIZABLE);
        
    } else {
        // Switch to fullscreen mode
        gameData.ecran = SDL_SetVideoMode(gameData.screenWidth, gameData.screenHeight, 32, SDL_HWSURFACE | SDL_FULLSCREEN);
        
    }
    }
     

}
                    if (gameData.playmenu == AFFBG){
                        
                   if (event.button.x >= gameData.select1playerpos.x && event.button.x <= gameData.select1playerpos.x + gameData.select1playerpos.w &&
                        event.button.y >= gameData.select1playerpos.y && event.button.y <= gameData.select1playerpos.y + gameData.select1playerpos.h) { 
                         gameData.showx = 1;
                         gameData.playmenu = PLAYER1;
                         gameData.sound=1;
                        }     
                       }
                        
                }
                
                if (gameData.playmenu == PLAYER1){
                 if (event.button.x >= gameData.selectcharacterpos.x && event.button.x <= gameData.selectcharacterpos.x + gameData.selectcharacterpos.w &&
                        event.button.y >= gameData.selectcharacterpos.y && event.button.y <= gameData.selectcharacterpos.y + gameData.selectcharacterpos.h){
                        
                         
                        gameData.chaselect1 = 1;
                        gameData.chaselect2 = 0;
                        
                        perso.numperso = 1;
                        m.numpersoo=1;
                        gameData.sound=1;
                        }
                        }
                  if (gameData.playmenu == PLAYER1){
                  if (event.button.x >= gameData.selectcharacterpos2.x && event.button.x <= gameData.selectcharacterpos2.x + gameData.selectcharacterpos2.w &&
                        event.button.y >= gameData.selectcharacterpos2.y && event.button.y <= gameData.selectcharacterpos2.y + gameData.selectcharacterpos2.h)
                        {
                        
                         
                        gameData.chaselect2 = 1;
                        gameData.chaselect1 = 0;
                        perso.numperso = 2;
                        m.numpersoo=2;
                        gameData.sound=1;
                        } 
                        }   
                        
                       if (gameData.playmenu == INGAME){
                        if (event.button.y >= 230 && event.button.y < 240 && event.button.x >= 150 && event.button.x <=470) {
                            printf("Réponse correcte !\n");
                            test= 1;
                            
                        } else if (event.button.y >= 270 && event.button.y < 290 && event.button.x >= 150 && event.button.x <=420) {
                              printf("Mauvaise réponse.\n");
                              gameData.quitter = 0;
                          } else if (event.button.y >= 330 && event.button.y < 340 && event.button.x >= 150 && event.button.x <=370) {
                                printf("Mauvaise réponse.\n");
                                gameData.quitter = 0;
                            }
                            }
                            
                            
                            if (gameData.playmenu == INGAME){
                            if (b.camera.x >= 750 && b.camera.x <= 950){
    if (event.button.x >= b3.NPCfixed.x - b.camera.x && event.button.x <= b3.NPCfixed.x - b.camera.x + 100 &&
                        event.button.y >= b3.NPCfixed.y - b.camera.y && event.button.y <= b3.NPCfixed.y - b.camera.y + 100) {
                          //if (!talkDisplayed) {
                         displaytalk = true;
                          //talkDisplayed = true;
                         // }
                        }
                        
                        }
                      if (b.camera.x >= 950 || b.camera.x <= 750 )
                      displaytalk = false;
                      zoomVisible = false;
                      zoom3visible =0;
                        }
                        
                        
                        if (event.button.x >= destRect.x + 47 && event.button.x <= destRect.x + 47 + 34 &&
        event.button.y >= destRect.y + 41 && event.button.y <= destRect.y + 77 && displaytalk) {
        
        zoomVisible = true; 

    }
    
    if (event.button.x >= zoompos.x + 47 && event.button.x <= zoompos.x + 322 &&
        event.button.y >= zoompos.y && event.button.y <= zoompos.y + 34 && displaytalk && zoom2visible) {
        zoom3visible =1;
        zoom2visible=0;
        zoomVisible = false;
        }
           
           if (event.button.x >= destRect.x +86 && event.button.x <= destRect.x+86 +34  &&
           event.button.y >= destRect.y+41 && event.button.y <= destRect.y + 77 && displaytalk) {
           if (!talkDisplayed) {
             showconv =1;
             talkDisplayed = true;
           }
           }
           
                       
                break;
              case SDL_MOUSEMOTION:
                   
                       if (event.button.x >= gameData.dstRect.x && event.button.x <= gameData.dstRect.x + gameData.dstRect.w &&
                        event.button.y >= gameData.dstRect.y && event.button.y <= gameData.dstRect.y + gameData.dstRect.h) {
                        
                             gameData.play=1;
                    }else {
        gameData.play = 0;
    }
    if (event.button.x >= gameData.dstRectp.x && event.button.x <= gameData.dstRectp.x + gameData.dstRectp.w &&
                        event.button.y >= gameData.dstRectp.y && event.button.y <= gameData.dstRectp.y + gameData.dstRectp.h) {
                         gameData.setting=1;
                    }else {
        gameData.setting = 0;
                    }
                      if (event.button.x >= gameData.dstRect2.x && event.button.x <= gameData.dstRect2.x + gameData.dstRect2.w &&
                        event.button.y >= gameData.dstRect2.y && event.button.y <= gameData.dstRect2.y + gameData.dstRect2.h) {
                        gameData.exit=1;
                          }else {
        gameData.exit = 0;
                    }
               if (event.button.x >= gameData.dstRectplus.x && event.button.x <= gameData.dstRectplus.x + gameData.dstRectplus.w &&
                        event.button.y >= gameData.dstRectplus.y && event.button.y <= gameData.dstRectplus.y + gameData.dstRectplus.h) {
                                    gameData.plus2=1;
                    }else {
        gameData.plus2 = 0;
                        }
                    if (event.button.x >= gameData.dstRectmoin.x && event.button.x <= gameData.dstRectmoin.x + gameData.dstRectmoin.w &&
                        event.button.y >= gameData.dstRectmoin.y && event.button.y <= gameData.dstRectmoin.y + gameData.dstRectmoin.h) {
                                    gameData.moin2=1;
                    }else {
        gameData.moin2 = 0;
                        }    
                      
                   if (event.button.x >= gameData.dstRectpp.x && event.button.x <= gameData.dstRectpp.x + gameData.dstRectpp.w &&
                        event.button.y >= gameData.dstRectpp.y && event.button.y <= gameData.dstRectpp.y + gameData.dstRectpp.h) { 
                                    gameData.fullscreen2=1;
                    }else {
        gameData.fullscreen2 = 0;
                        }
                         if (event.button.x >= gameData.dstRectf.x && event.button.x <= gameData.dstRectf.x + gameData.dstRectf.w &&
                        event.button.y >= gameData.dstRectf.y && event.button.y <= gameData.dstRectf.y + gameData.dstRectf.h ||
                       event.button.x >= gameData.dstRectf2.x && event.button.x <= gameData.dstRectf2.x + gameData.dstRectf2.w &&
                        event.button.y >= gameData.dstRectf2.y && event.button.y <= gameData.dstRectf2.y + gameData.dstRectf2.h ) {
                                        gameData.x=1;
                    }else {
        gameData.x = 0;
                    }  
                    
                    if (event.button.x >= gameData.dstRectpe.x && event.button.x <= gameData.dstRectpe.x + gameData.dstRectpe.w &&
                        event.button.y >= gameData.dstRectpe.y && event.button.y <= gameData.dstRectpe.y + gameData.dstRectpe.h) {
                        
                             gameData.pe=1;
                    }else {
        gameData.pe = 0;
    }
    
       if (event.button.x >= gameData.dstRectc.x && event.button.x <= gameData.dstRectc.x + gameData.dstRectc.w &&
                        event.button.y >= gameData.dstRectc.y && event.button.y <= gameData.dstRectc.y + gameData.dstRectc.h) {
                        
                             gameData.credit=1;
                    }else {
        gameData.credit = 0;
    }
    
     if (event.button.x >= gameData.select1playerpos.x && event.button.x <= gameData.select1playerpos.x + gameData.select1playerpos.w &&
                        event.button.y >= gameData.select1playerpos.y && event.button.y <= gameData.select1playerpos.y + gameData.select1playerpos.h) {
                        
                             gameData.showselect1player=1;
                    }else {
        gameData.showselect1player = 0;
    }
    
    if (event.button.x >= gameData.select2playerpos.x && event.button.x <= gameData.select2playerpos.x + gameData.select2playerpos.w &&
                        event.button.y >= gameData.select2playerpos.y && event.button.y <= gameData.select2playerpos.y + gameData.select2playerpos.h) {
                        
                             gameData.showselect2player=1;
                    }else {
        gameData.showselect2player = 0;
    }
    
    
    if (event.button.x >= destRect.x +86 && event.button.x <= destRect.x+86 +34  &&
           event.button.y >= destRect.y+41 && event.button.y <= destRect.y + 77 && displaytalk) {
                  
                  showtalk =1;
                }
                else
                showtalk =0;
                
                
                
                if (event.button.x >= destRect.x +47 && event.button.x <= destRect.x+47 +34  &&
           event.button.y >= destRect.y+41 && event.button.y <= destRect.y + 77 && displaytalk) {
                  
                  showtalk2 =1;
                }
                else
                showtalk2 =0;
                
                
                if (event.button.x >= zoompos.x + 47 && event.button.x <= zoompos.x + 322 &&
        event.button.y >= zoompos.y && event.button.y <= zoompos.y + 34 && displaytalk && !zoom3visible && zoomVisible) {
        zoom2visible = 1;
        }
        else
        zoom2visible = 0;
               break; 
               case SDL_MOUSEBUTTONUP :
                                                a=event.button.x/190;
				    		bb=event.button.y/190;
				    		coup=3*bb+a;
				    		t.tour++;
               break; 
    }
}

Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time = current_time - start_time;
       
        SDL_FillRect(gameData.ecran, NULL, 0);
        
         
        
        static int lastFrameTime = 0;
        int currentFrameTime = SDL_GetTicks();
        int frameDuration = 250;  
        if (currentFrameTime - lastFrameTime >= frameDuration) {
    gameData.currentImage = (gameData.currentImage + 1) % 2;
    gameData.position2 = (gameData.position2 + 1) % 2;
    gameData.cigara = (gameData.cigara + 1) % 9;
    lastFrameTime = currentFrameTime;
}

if (gameData.menuState == MAIN_MENU || gameData.menuState == OPTIONS_MENU || gameData.playmenu == AFFBG ) {
    SDL_BlitSurface(gameData.images[gameData.currentImage], NULL, gameData.ecran, &gameData.positionImage);
    SDL_BlitSurface(gameData.animation[gameData.currentImage], NULL, gameData.ecran, &gameData.machineposition);
    SDL_BlitSurface(gameData.animation2[gameData.currentImage], NULL, gameData.ecran, &gameData.positiontop[gameData.position2]);
    SDL_BlitSurface(gameData.cigar[gameData.cigara], NULL, gameData.ecran, &gameData.cigarpos);
}


if (keys[SDLK_LEFT] && !showpressj && !displayIntroImage && !showhand && !showconv) {
      if (perso.posScreen.x <= 50)
            scrolling(&b, 1, 5);
            
            if (ctrl_pressed) {
            if (perso.posScreen.x <= 50)
            scrolling(&b, 1, 7);
            
            }
           if (collisionPP(b.camera, m.masque)) { 
                b.camera.x += 5; 
            }  
        }
        if (keys[SDLK_RIGHT] && !showpressj && !displayIntroImage && !showhand && !showconv) {
            scrolling(&b, 0, 5);
            if (ctrl_pressed) {
            scrolling(&b, 0, 7);
            
            }
           if (collisionPP(b.camera, m.masque)) { 
                b.camera.x -= 5; 
            }  
        }
        /*if (keys[SDLK_UP]) {
            scrolling(&b, 2, 10);
        }
        if (keys[SDLK_DOWN]) {
            scrolling(&b, 3, 10);
        }*/
        if (keys[SDLK_q]) {
            scrolling(&b2, 1, 10);
        }
        if (keys[SDLK_d]) {
            scrolling(&b2, 0, 10);
        }
        if (keys[SDLK_z]) {
            scrolling(&b2, 2, 10);
        }
        if (keys[SDLK_s]) {
            scrolling(&b2, 3, 10);
        }
drawButtons(&gameData);
if (gameData.playmenu == INGAME){
    if (!inGame) {
            // Reset the start time when entering the INGAME state
            start_time = SDL_GetTicks();
            inGame = true;
        }    
if (multiplayerMode) {

            afficherBackPartage(b, b2, gameData.ecran);
            animerBack(&b3,gameData.ecran,&b);
            if (b.camera.x >= 1560 ){
            moveperso(&perso,&b,m.masque);
            MAJMinimap(perso.posScreen,&m,b.camera,11,1500, 800);
           }
            saut(&perso, &flag,perso.numperso);
            animerperso(&perso,perso.numperso);
            afficherPerso(perso,gameData.ecran,perso.numperso);
            
            
             MAJMinimap(perso.posScreen,&m,b.camera,11,1120, 800);
             afficher_minimap(m,gameData.ecran,perso.numperso);
             
            
             //affichertempsen(elapsed_time / 1000);
             affichertemps(elapsed_time / 1000);
             animerMinimape(&m);
        } else {
        
            afficherBack(b, gameData.ecran);
            animerBack(&b3,gameData.ecran,&b);
            if (b.camera.x >= 1560  ){
            moveperso(&perso,&b,m.masque);
            MAJMinimap(perso.posScreen,&m,b.camera,11,1120, 800);
           }
           else{
           MAJMinimap(perso.posScreen,&m,b.camera,11,1120, 800);
           }
              
             saut(&perso, &flag,perso.numperso);
             animerperso(&perso,perso.numperso);
             
             afficherPerso(perso,gameData.ecran,perso.numperso); 
 
             afficher_minimap(m,gameData.ecran,perso.numperso);
             
             //affichertempsen(elapsed_time / 1000);
             affichertemps(elapsed_time / 1000);
             animerMinimape(&m);
             if (collisionPP(perso.posScreen, m.masque)) {
                perso.posScreen.y += 10; 
            }
        }
        
        
        if (perso.posScreen.x >=600 && perso.posScreen.x <=650){
          if (test == 1){
            animerEnig(&e,gameData.ecran);
            SDL_Delay(200);
                
             }
          if (test == 0)
            afficherEnigme(e2,gameData.ecran ); 
    
            }
          
          if (perso.posScreen.x >=1080){
          affichage(t,gameData.ecran);
          if( t.tour<9 && atilganer(t.tabsuivi)==0)
		{
			if((t.tour+joueur)%2==0)//tour du PC
            		{
				calcul_coup(t.tabsuivi);
 				t.tour++;
			}
			else
			{
				
                          t.tabsuivi[coup]=-1;
    				
			}

		}
		else
		{ 
			Resultat(t,gameData.ecran);
 			 
			printf("%d", t.tour);
		}
          }
          if (perso.numperso == 1){
          if (displayIntroImage) 
          SDL_BlitSurface(introImage, NULL, gameData.ecran, &introImagePosition);
         }
         else if (perso.numperso == 2){
          if (displayIntroImage2) 
          SDL_BlitSurface(introImage2, NULL, gameData.ecran, &introImagePosition);
         }
         
         if (displaytalk){
         SDL_BlitSurface(talk, NULL, gameData.ecran, &destRect);
         }
         
         if (showtalk ==1)
         SDL_BlitSurface(talk2, NULL, gameData.ecran, &destRect);
         
         if (showtalk2 ==1)
         SDL_BlitSurface(talk3, NULL, gameData.ecran, &destRect);
         
         if (zoomVisible)
         SDL_BlitSurface(zoom, NULL, gameData.ecran, &zoompos);
         if (zoom2visible)
         SDL_BlitSurface(zoom2, NULL, gameData.ecran, &zoompos);
         if (zoom3visible)
         SDL_BlitSurface(zoom3, NULL, gameData.ecran, &zoompos);
         
         if (showconv)
         SDL_BlitSurface(conv, NULL, gameData.ecran, &zoompos);
         
         if (showconv1)
         SDL_BlitSurface(conv2, NULL, gameData.ecran, &zoompos);
         
         if (showconv2)
         SDL_BlitSurface(conv3, NULL, gameData.ecran, &zoompos);
         if (showconv3)
         SDL_BlitSurface(conv4, NULL, gameData.ecran, &zoompos);
         if (showpressj)
         SDL_BlitSurface(pressj, NULL, gameData.ecran, &pressjpos);
         if (showhand)
         SDL_BlitSurface(hand, NULL, gameData.ecran, &handpos);
}
         SDL_Flip(gameData.ecran);
       
    }
	SDL_FreeSurface(b.backgroundImg);
    cleanupGame(&gameData);
    liberer_minimap(&m);
    liberationmemoire(t);


    return 0;
    
} 
                     
