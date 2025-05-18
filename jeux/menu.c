#include "menu.h"

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_events.h>

void initializeGame(struct GameData* gameData) {


    gameData->quitter = 1;
    // Initialize SDL video and audio subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Echec d'initialisation de SDL : %s\n", SDL_GetError());
         exit(1);
    }
    
       
   
      // Create a window with a display area of 1120*800 pixels
    gameData->ecran = SDL_SetVideoMode(1120, 800, 32, SDL_HWSURFACE | SDL_RESIZABLE);
    if (gameData->ecran == NULL) {
        fprintf(stderr, "Echec de creation de la fenetre : %s.\n", SDL_GetError());
         exit(1);
    }
    
    gameData->menuState = MAIN_MENU;
    
    gameData->currentImage = 0;
     gameData->specificBoxClicked = 1;
      
     
      gameData->showx = 1;
      gameData->showoption=0;
      gameData->play=0;
       gameData->setting=0;
    gameData-> exit=0;
    gameData->x=0;
    gameData->plus2=0;
    gameData->moin2=0;
    gameData->fullscreen2=0;
     gameData->sound=0;
      gameData->b=400;
      gameData->pleinecran=0;
      gameData->isFullscreen = 0;
      gameData->pe=0;
     gameData->credit=0;
    gameData->flou=0;
    gameData->showxflou =0;
      gameData->soundVolume = 50;
      gameData->showmenu = 0;
      gameData->bgs=0;
      
      gameData->showselect1player = 0;
      gameData->showselect2player = 0;
      
      gameData->chaselect1 = 0;
      gameData->chaselect2 = 0;
      
      gameData->b = 650;
      
     gameData->screenWidth = 1120; 
     gameData->screenHeight = 800;
      
      
      
      
    // Load the images
    loadImages(gameData);




    // Set the position of the image
    gameData->positionImage.x = 0;
    gameData->positionImage.y = 0;


    // load sound 
     gameData->s[0] = IMG_Load("optionimg/s0.png");
    gameData->s[1] = IMG_Load("optionimg/s1.png");
    gameData->s[2] = IMG_Load("optionimg/s2.png");
     gameData->s[3] = IMG_Load("optionimg/s3.png");
    gameData->s[4] = IMG_Load("optionimg/s4.png");
    gameData->s[5] = IMG_Load("optionimg/s5.png");
     gameData->s[6] = IMG_Load("optionimg/s6.png");
    gameData->s[7] = IMG_Load("optionimg/s7.png");
    gameData->s[8] = IMG_Load("optionimg/s8.png");
     gameData->s[9] = IMG_Load("optionimg/s9.png");
    gameData->s[10] = IMG_Load("optionimg/s10.png");
    

    //load animation
      gameData->animation[0] = IMG_Load("bg menu/machine1.png");
    gameData->animation[1] = IMG_Load("bg menu/machine2.png");
    gameData->animation2[0] = IMG_Load("bg menu/top.png");
    gameData->animation2[1] = IMG_Load("bg menu/top2.png");
     
    
    
    //load buttons
    gameData->buttons[0] = IMG_Load("bg menu/jouer.png");
    gameData->buttons[1] = IMG_Load("bg menu/option.png");
    gameData->buttons[2] = IMG_Load("bg menu/quitter.png");
      gameData->buttons2[0] = IMG_Load("bg menu/jouer2.png");
    gameData->buttons2[1] = IMG_Load("bg menu/option2.png");
    gameData->buttons2[2] = IMG_Load("bg menu/quitter2.png");
    gameData->buttons[3] = IMG_Load("bg menu/credits.png");
    gameData->buttons2[3] = IMG_Load("bg menu/credits2.png");
    
     gameData->option[0] = IMG_Load("optionimg/windo.png");
     gameData->xbutton = IMG_Load("bg menu/x.png");
     gameData->xbutton2 = IMG_Load("bg menu/x2.png");
     gameData->option[1] = IMG_Load("optionimg/plus.png");
     gameData->option[2] = IMG_Load("optionimg/moin.png");
     gameData->option2[0] = IMG_Load("optionimg/plus2.png");
     gameData->option2[1] = IMG_Load("optionimg/moin2.png");
     gameData->option[3] = IMG_Load("optionimg/p.png");
     gameData->option2[2] = IMG_Load("optionimg/p2.png");
     gameData->option[4] = IMG_Load("optionimg/pe.png");
     gameData->option[5] = IMG_Load("optionimg/pe2.png");
     
     gameData->videos = IMG_Load("optionimg/video.png");
     
    
    
     gameData->menuplay = IMG_Load("playimg/playmenu.png");
     
     gameData->character1 = IMG_Load("playimg/character1.png");
     gameData->character2 = IMG_Load("playimg/character2.png");
     
     
     gameData->omnistrike[0] = IMG_Load("playimg/omnistrike1.png");
     gameData->omnistrike[1] = IMG_Load("playimg/omnistrike2.png");
     gameData->omnistrike[2] = IMG_Load("playimg/omnistrike3.png");
     
     
     gameData->rise[0] = IMG_Load("playimg/rise1.png");
     gameData->rise[1] = IMG_Load("playimg/rise2.png");
     gameData->rise[2] = IMG_Load("playimg/rise3.png");
     
     
     gameData->select1player[0] = IMG_Load("playimg/1player1.png");
     gameData->select1player[1] = IMG_Load("playimg/1player2.png");
     
     gameData->select2player[0] = IMG_Load("playimg/2player1.png");
     gameData->select2player[1] = IMG_Load("playimg/2player2.png");
     
     
     
     gameData->bgselect = IMG_Load("playimg/bgselect.png");
     
     gameData->textcharacter = IMG_Load("playimg/characterselect.png");
     
     gameData->selectcharacter[0] = IMG_Load("playimg/ch1.png");
     gameData->selectcharacter[1] = IMG_Load("playimg/ch2.png");
     
     
     gameData->MaxHarper[0] = IMG_Load("playimg/maxharper1.png");
     gameData->MaxHarper[1] = IMG_Load("playimg/maxharper2.png");
     initText(gameData);
     
     gameData->selectch1design[0] = IMG_Load("playimg/ch1design1.png");
     gameData->selectch1design[1] = IMG_Load("playimg/ch1design2.png");
     gameData->selectch1design[2] = IMG_Load("playimg/ch1design3.png");
     
     gameData->selectch1design2[0] = IMG_Load("playimg/ch2design1.png");
     gameData->selectch1design2[1] = IMG_Load("playimg/ch2design2.png");
     gameData->selectch1design2[2] = IMG_Load("playimg/ch2design3.png");
     
     gameData->PRESS[0] = IMG_Load("playimg/press1.png");
     gameData->PRESS[1] = IMG_Load("playimg/press2.png");
     gameData->PRESS[2] = IMG_Load("playimg/press3.png");
     
     
     gameData->PRESSP = IMG_Load("playimg/PressP.png");
     
     gameData->typeName = IMG_Load("playimg/typename.png");
     
     
     
      
     
    
     for (int i = 0; i < 2; ++i) {
      if (gameData->animation[i] == NULL) {
          fprintf(stderr, "Echec de chargement de l'image du animation %d : %s.\n", i + 1, IMG_GetError());
           exit(1);
      }
    }
    for (int i = 0; i < 11; ++i) {
      if (gameData->s[i] == NULL) {
          fprintf(stderr, "Echec de chargement de l'image du s %d : %s.\n", i + 1, IMG_GetError());
           exit(1);
      }
    }
    for (int i = 0; i < 4; ++i) {
      if (gameData->buttons[i] == NULL) {
          fprintf(stderr, "Echec de chargement de l'image du bouton %d : %s.\n", i + 1, IMG_GetError());
           exit(1);
      }
    }
      for (int i = 0; i < 5; ++i) {
        if (gameData->option[i] == NULL) {
           fprintf(stderr, "Echec de chargement de l'image du bouton %d : %s.\n", i + 1, IMG_GetError());
           exit(1);
        }
    }

if (gameData->xbutton == NULL) {
        fprintf(stderr, "Echec de chargement de l'image du x: %s.\n",  IMG_GetError());
         exit(1);
    }
    if (gameData->xbutton2 == NULL) {
        fprintf(stderr, "Echec de chargement de l'image du x: %s.\n",  IMG_GetError());
         exit(1);
    }
    
    
     if (gameData->videos == NULL) {
        fprintf(stderr, "Echec de chargement de l'image du video: %s.\n",  IMG_GetError());
         exit(1);
    }
  
 
  //animation

//machine
gameData->machineposition.x = 950;
gameData->machineposition.y = 300;
//top
gameData->positiontop[0].x = 450;
gameData->positiontop[0].y = 50;
//top2
gameData->positiontop[1].x = 550;
gameData->positiontop[1].y = 50;




// position de sound
gameData->positionsound.x = 300;
gameData->positionsound.y = 230;
  

// bg option
gameData->postionoption.x = 185;
gameData->postionoption.y = 90;

  //plus
gameData->dstRectplus.x = 630;
gameData->dstRectplus.y = 330;

//moin
gameData->dstRectmoin.x = 550;
gameData->dstRectmoin.y = 330;

//plein ecran
gameData->dstRectpp.x = 550;
gameData->dstRectpp.y = 500;

//pe


gameData->dstRectpe.x = 550;
gameData->dstRectpe.y = 500;  
//menu 

//position x button 
gameData->dstRectf.x = 900;
gameData->dstRectf.y = 80;  

// position de mousemotion  pour les button 


// button positions
gameData->dstRect.x = 500;
gameData->dstRect.y = 200;


gameData->dstRectp.x = 500;
gameData->dstRectp.y = 350;

gameData->dstRectc.x = 500;
gameData->dstRectc.y = 500;

gameData->dstRect2.x = 500;
gameData->dstRect2.y = 500;


// position de video sur option
gameData->videopos.x = 300;
gameData->videopos.y = 500;


//position de cigarette
gameData->cigarpos.x = 390;
gameData->cigarpos.y = 383;

//position de l'image flou
gameData->flouRect.x = 0;
gameData->flouRect.y = 0;

gameData->c1.x = 300;
gameData->c1.y = 300;

//position menuplay
gameData->menuplaypos.x = 400;
gameData->menuplaypos.y = 50;

//omnistrikepos
gameData->omnistrikepos.x = 450;
gameData->omnistrikepos.y = 100;

//rise position
gameData->risepos.x = 420;
gameData->risepos.y = 150;


//select 1player pos
gameData->select1playerpos.x = 490;
gameData->select1playerpos.y = 350;
gameData->select1playerpos2.x = 452;
gameData->select1playerpos2.y = 343;

//select 2player pos
gameData->select2playerpos.x = 490;
gameData->select2playerpos.y = 500;
gameData->select2playerpos2.x = 452;
gameData->select2playerpos2.y = 493;

//bgselect 1player pos
gameData->bgselectpos.x = 0;
gameData->bgselectpos.y = 0;


//textcharacter select pos
gameData->textcharacterpos.x = 70;
gameData->textcharacterpos.y = 50;

//select character pos
gameData->selectcharacterpos.x = 200;
gameData->selectcharacterpos.y = 200;
gameData->selectcharacterpos2.x = 600;
gameData->selectcharacterpos2.y = 200;


//max harper character select pos
gameData->MaxHarperpos[0].x = 260;
gameData->MaxHarperpos[0].y = 250;
gameData->MaxHarperpos[1].x = 650;
gameData->MaxHarperpos[1].y = 240;

// press position
gameData->PRESSpos.x = 270;
gameData->PRESSpos.y = 650;
// press p
gameData->PRESSPpos.x = 500;
gameData->PRESSPpos.y = 650;

//type name position
gameData->typeNamepos.x = 300;
gameData->typeNamepos.y = 100;






    // Initialize the audio 
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        fprintf(stderr, "Echec d'initialisation de l'audio : %s\n", Mix_GetError());
         exit(1);
    }

    // Load the music file 
    gameData->musique = Mix_LoadMUS("music/arcade.mp3");
    gameData->sonb = Mix_LoadWAV ("music/bouton.wav");
    
     if (gameData->sonb == NULL) {
        fprintf(stderr, "Echec de chargement de la sonb : %s\n", Mix_GetError());
         exit(1);
    }
    if (gameData->musique == NULL) {
        fprintf(stderr, "Echec de chargement de la musique : %s\n", Mix_GetError());
         exit(1);
    }

    // Play the music in a loop
     Mix_VolumeMusic(gameData->soundVolume);
    Mix_PlayMusic(gameData->musique, -1);
   

 gameData->showButtons = 1;   






 
}




void drawButtons(struct GameData* gameData) {
      
    if(gameData->sound){
       Mix_PlayChannel(-1, gameData->sonb, 0);
       gameData->sound=0; 
    }
       
        switch (gameData->menuState) {
        
 
        
        case MAIN_MENU:
        
        
        renderText(gameData, "OmniStrike :  Rise of the Resistance", 100, 750, 48, (SDL_Color){255, 255, 255, 255});  
        SDL_BlitSurface(gameData->buttons[0], NULL, gameData->ecran, &gameData->dstRect);
        SDL_BlitSurface(gameData->buttons[1], NULL, gameData->ecran, &gameData->dstRectp);
        SDL_BlitSurface(gameData->buttons[2], NULL, gameData->ecran, &gameData->dstRect2);
        //SDL_BlitSurface(gameData->buttons[3], NULL, gameData->ecran, &gameData->dstRectc);
        if (gameData->play){
        SDL_BlitSurface(gameData->buttons2[0], NULL, gameData->ecran, &gameData->dstRect);
        }
         if (gameData->setting){
        SDL_BlitSurface(gameData->buttons2[1], NULL, gameData->ecran, &gameData->dstRectp);
        } 
         if (gameData->exit){
        SDL_BlitSurface(gameData->buttons2[2], NULL, gameData->ecran, &gameData->dstRect2);
        } 
            break;
            
            
         // option 
         
            
        case OPTIONS_MENU:
        

            renderText(gameData, "OmniStrike :  Rise of the Resistance", 100, 750, 48, (SDL_Color){255, 255, 255, 255});  
          switch(gameData->showx){
          case 1 :
          
   
             SDL_BlitSurface(gameData->xbutton, NULL, gameData->ecran, &gameData->dstRectf);
             if (gameData->x){
             SDL_BlitSurface(gameData->xbutton2, NULL, gameData->ecran, &gameData->dstRectf);
             }
             
             
    SDL_BlitSurface(gameData->option[0], NULL, gameData->ecran, &gameData->postionoption);
    SDL_BlitSurface(gameData->option[1], NULL, gameData->ecran, &gameData->dstRectplus);
    SDL_BlitSurface(gameData->option[2], NULL, gameData->ecran, &gameData->dstRectmoin);
    SDL_BlitSurface(gameData->option[3], NULL, gameData->ecran, &gameData->dstRectpp);
    if (gameData->isFullscreen) {
        SDL_BlitSurface(gameData->option[4], NULL, gameData->ecran, &gameData->dstRectpe);
    } else {
        SDL_BlitSurface(gameData->option[3], NULL, gameData->ecran, &gameData->dstRectpp);
    }

    SDL_BlitSurface(gameData->s[gameData->soundVolume / 10], NULL, gameData->ecran, &gameData->positionsound);

    SDL_BlitSurface(gameData->videos, NULL, gameData->ecran, &gameData->videopos);

    if (gameData->plus2) {
        SDL_BlitSurface(gameData->option2[0], NULL, gameData->ecran, &gameData->dstRectplus);
    }
    if (gameData->moin2) {
        SDL_BlitSurface(gameData->option2[1], NULL, gameData->ecran, &gameData->dstRectmoin);
    }
      if (gameData->fullscreen2) {
            SDL_BlitSurface(gameData->option2[2], NULL, gameData->ecran, &gameData->dstRectpp);
        }

        
        
    
     break; 
}
  break;
      
       
        
         
           
        
         //play   
           
        case PLAY_MENU:
        switch(gameData->showx){
          case 1 :
             SDL_BlitSurface(gameData->xbutton, NULL, gameData->ecran, &gameData->dstRectf);
             if (gameData->x){
             SDL_BlitSurface(gameData->xbutton2, NULL, gameData->ecran, &gameData->dstRectf);
             }
            
        switch(gameData->playmenu){
             case AFFBG:
             renderText(gameData, "OmniStrike :  Rise of the Resistance", 100, 750, 48, (SDL_Color){255, 255, 255, 255});  
        SDL_BlitSurface(gameData->menuplay, NULL, gameData->ecran, &gameData->menuplaypos);
        
         int start = SDL_GetTicks();
        static int end = 0;
        static int ppp = 0;

        if (start - end >= 350) {
            ppp = (ppp + 1) % 3;
            end = start;
        }
        
        SDL_BlitSurface(gameData->omnistrike[ppp], NULL, gameData->ecran, &gameData->omnistrikepos);
        SDL_BlitSurface(gameData->rise[ppp], NULL, gameData->ecran, &gameData->risepos);
        SDL_UpdateRect(gameData->ecran, gameData->omnistrikepos.x, gameData->omnistrikepos.y,
                       gameData->omnistrikepos.w, gameData->omnistrikepos.h);
        SDL_UpdateRect(gameData->ecran, gameData->risepos.x, gameData->risepos.y,
                       gameData->risepos.w, gameData->risepos.h);
                       
        SDL_BlitSurface(gameData->select1player[0], NULL, gameData->ecran, &gameData->select1playerpos);   
        SDL_BlitSurface(gameData->select2player[0], NULL, gameData->ecran, &gameData->select2playerpos);    
        if (gameData->showselect1player){
        SDL_BlitSurface(gameData->select1player[1], NULL, gameData->ecran, &gameData->select1playerpos2); 
        }  
        if (gameData->showselect2player){
        SDL_BlitSurface(gameData->select2player[1], NULL, gameData->ecran, &gameData->select2playerpos2); 
        }    
        
            
        //SDL_BlitSurface(gameData->character1, NULL, gameData->ecran, &gameData->c1);
        //SDL_BlitSurface(gameData->menuplay, NULL, gameData->ecran, &gameData->postionoption);
            break;
            case PLAYER1:
            renderText(gameData, "OmniStrike :  Rise of the Resistance", 100, 750, 48, (SDL_Color){255, 255, 255, 255});  
            gameData->dstRectf2.x = 1000;
            gameData->dstRectf2.y = 70; 
            SDL_BlitSurface(gameData->bgselect, NULL, gameData->ecran, &gameData->bgselectpos);
           SDL_BlitSurface(gameData->xbutton, NULL, gameData->ecran, &gameData->dstRectf2);
             if (gameData->x){
             SDL_BlitSurface(gameData->xbutton2, NULL, gameData->ecran, &gameData->dstRectf2);
             }
             SDL_BlitSurface(gameData->textcharacter, NULL, gameData->ecran, &gameData->textcharacterpos);
             SDL_BlitSurface(gameData->selectcharacter[0], NULL, gameData->ecran, &gameData->selectcharacterpos);
             SDL_BlitSurface(gameData->selectcharacter[1], NULL, gameData->ecran, &gameData->selectcharacterpos2);
             
             SDL_BlitSurface(gameData->MaxHarper[0], NULL, gameData->ecran, &gameData->MaxHarperpos[0]);
             SDL_BlitSurface(gameData->MaxHarper[1], NULL, gameData->ecran, &gameData->MaxHarperpos[1]);
             
              int start2 = SDL_GetTicks();
        static int end2 = 0;
        static int ppp2 = 0;

        if (start2 - end2 >= 150) {
            ppp2 = (ppp2 + 1) % 3;
            end2 = start2;
        }
        SDL_BlitSurface(gameData->PRESS[ppp2], NULL, gameData->ecran, &gameData->PRESSpos);
             
             if (gameData->chaselect1){
             int start = SDL_GetTicks();
        static int end = 0;
        static int ppp = 0;

        if (start - end >= 150) {
            ppp = (ppp + 1) % 3;
            end = start;
        }
             SDL_BlitSurface(gameData->selectch1design[ppp], NULL, gameData->ecran, &gameData->selectcharacterpos);
             SDL_BlitSurface(gameData->MaxHarper[0], NULL, gameData->ecran, &gameData->MaxHarperpos[0]);
             
             }
             
             if (gameData->chaselect2){
             int start = SDL_GetTicks();
        static int end = 0;
        static int ppp = 0;

        if (start - end >= 150) {
            ppp = (ppp + 1) % 3;
            end = start;
        }
             SDL_BlitSurface(gameData->selectch1design2[ppp], NULL, gameData->ecran, &gameData->selectcharacterpos2);
             SDL_BlitSurface(gameData->MaxHarper[1], NULL, gameData->ecran, &gameData->MaxHarperpos[1]);
             
             }
             
             if(gameData->b>600){
             gameData->b=gameData->b-5;
             }else{
             gameData->b=650;
             }
             gameData->PRESSPpos.y=gameData->b;
             
           SDL_BlitSurface(gameData->PRESSP, NULL, gameData->ecran, &gameData->PRESSPpos); 
        
        
             
            break;
            
            
            case typen:
            
      
    
            
            SDL_BlitSurface(gameData->bgselect, NULL, gameData->ecran, &gameData->bgselectpos);
            SDL_BlitSurface(gameData->xbutton, NULL, gameData->ecran, &gameData->dstRectf2);
             if (gameData->x){
             SDL_BlitSurface(gameData->xbutton2, NULL, gameData->ecran, &gameData->dstRectf2);
             }
            SDL_BlitSurface(gameData->typeName, NULL, gameData->ecran, &gameData->typeNamepos);
      
     
        renderText2(gameData, gameData->playerName, 350, 270, 48, (SDL_Color){255, 255, 255, 255});
    renderText(gameData, "OmniStrike :  Rise of the Resistance", 100, 750, 48, (SDL_Color){255, 255, 255, 255});  
            
            break;
            
            
            
            
            

           
            
        }
        
        break;
         }
        
            break;

  
        
       
    

            }
     
         
        
       
    }
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
       
    
void handleTypingEvent(struct GameData* gameData, SDL_Event event) {
    printf("%s\n",gameData->playerName);
   char inputChar;
        
        if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) {
            gameData->playerName[strlen(gameData->playerName)] = event.key.keysym.sym;
            gameData->playerName[strlen(gameData->playerName) + 1] = '\0';
        } else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(gameData->playerName) > 0) {
          
            gameData->playerName[strlen(gameData->playerName) - 1] = '\0';
        }
        else if (event.key.keysym.sym ==SDLK_SPACE){
        gameData->playerName[strlen(gameData->playerName)] = ' ';
        gameData->playerName[strlen(gameData->playerName) + 1] = '\0';
        }
        
   strncat(gameData->playerName, &inputChar, 1); 
}





 void loadImages(struct GameData* gameData) {
    char imagePath[100];
    for (int i = 0; i < 2; ++i) {
        sprintf(imagePath, "bg menu/bg%d.png", i + 1);
        gameData->images[i] = IMG_Load(imagePath);
        if (gameData->images[i] == NULL) {
            fprintf(stderr, "Echec de chargement de l'image %d : %s.\n", i + 1, IMG_GetError());
             exit(1);
        }
    }
    
    for (int j = 0; j < 9; ++j) {
        sprintf(imagePath, "bg menu/cigarette%d.png", j + 1);
        gameData->cigar[j] = IMG_Load(imagePath);
        if (gameData->cigar[j] == NULL) {
            fprintf(stderr, "Echec de chargement de cigar %d : %s.\n", j + 1, IMG_GetError());
             exit(1);
        }
    }
}

void cleanupGame(struct GameData* gameData) {
   
    for (int i = 0; i < 3; ++i) {
        SDL_FreeSurface(gameData->animation[i]);
    }
     for (int i = 0; i < 3; ++i) {
        SDL_FreeSurface(gameData->animation2[i]);
    }
    for (int i = 0; i < 3; ++i) {
        SDL_FreeSurface(gameData->images[i]);
    }
    
    for (int i = 0; i < 10; ++i) {
        SDL_FreeSurface(gameData->cigar[i]);
    }
    
        for (int i = 0; i < 12; ++i) {
    SDL_FreeSurface(gameData->s[i]);
}
    
    for (int i = 0; i < 4; ++i) {
    SDL_FreeSurface(gameData->buttons[i]);
}
  for (int i = 0; i < 4; ++i) {
    SDL_FreeSurface(gameData->buttons2[i]);
}
  for (int i = 0; i < 5; ++i) {
    SDL_FreeSurface(gameData->option[i]);
}
  for (int i = 0; i < 6; ++i) {
    SDL_FreeSurface(gameData->option2[i]);
}

SDL_FreeSurface(gameData->xbutton);
SDL_FreeSurface(gameData->xbutton2);
SDL_FreeSurface(gameData->menuplay);
for (int i = 0; i < 3; ++i) {
    SDL_FreeSurface(gameData->omnistrike[i]);
}

for (int i = 0; i < 3; ++i) {
    SDL_FreeSurface(gameData->rise[i]);
}
SDL_FreeSurface(gameData->bgselect);
for (int i = 0; i < 2; ++i) {
SDL_FreeSurface(gameData->select1player[i]);
}
for (int i = 0; i < 2; ++i) {
SDL_FreeSurface(gameData->select2player[i]);
}
SDL_FreeSurface(gameData->character1);
SDL_FreeSurface(gameData->textcharacter);
for (int i = 0; i < 2; ++i) {
SDL_FreeSurface(gameData->selectcharacter[i]);
}
for (int i = 0; i < 2; ++i) {
SDL_FreeSurface(gameData->MaxHarper[i]);
}

for (int i = 0; i < 3; ++i) {
SDL_FreeSurface(gameData->selectch1design[i]);
}

for (int i = 0; i < 3; ++i) {
SDL_FreeSurface(gameData->selectch1design2[i]);
}

for (int i = 0; i < 3; ++i) {
SDL_FreeSurface(gameData->PRESS[i]);
}


SDL_FreeSurface(gameData->PRESSP);
SDL_FreeSurface(gameData->typeName);





cleanupText(gameData);


    Mix_FreeMusic(gameData->musique);
    Mix_FreeChunk( gameData->sonb);
    
    Mix_CloseAudio();
   
    SDL_Quit();
}


 


    

  

   



