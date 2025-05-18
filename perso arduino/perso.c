#include "perso.h"
#include <SDL/SDL_image.h>
#include <stdbool.h>
#define GRAVITY 5
#define JUMP_SPEED 25 // Adjust as needed
#define MOVE_SPEED 5 // Adjust as needed
#define JUMP_FORCE 5 // Adjust as needed
/*void init(Personne *p, int numperso) {

    if (numperso == 1) {
        initperso(p);
    } else if (numperso == 2) {
       
    }
}*/



void initPerso(Personne *p) {

    p->idle = IMG_Load("Punk_idle.png");
    p->run = IMG_Load("Punk_run.png");
    p->runleft = IMG_Load("Punk_run_left.png");
    p->jump = IMG_Load("Punk_jump.png");
    p->jumpleft = IMG_Load("Punk_jumpleft.png");
    p->current_sprite = p->idle;
    
    p->speed=0;
    
    p->ground = 650;
     if (p->idle == NULL) {
        printf("Unable to load idle image! SDL_Error: %s\n", SDL_GetError());

        return;
    }
    p->direction = 0;
    p->posScreen.x = 300;
    p->posScreen.y = 650;
    p->posScreen.w = 0;
    p->posScreen.h = 0;
    
    p->posSprite.x = 0;
    p->posSprite.y = 0;
    p->posSprite.w = p->idle->w /4; 
    p->posSprite.h = p->idle->w ; 
    
    p->posSprite2.x = 0;
    p->posSprite2.y = 0;
    p->posSprite2.w = p->run->w /6; 
    p->posSprite2.h = p->run->w ; 
    
    p->posSprite3.x = 0;
    p->posSprite3.y = 0;
    p->posSprite3.w = p->runleft->w /6; 
    p->posSprite3.h = p->runleft->w ; 
    
    p->posSprite4.x = 0;
    p->posSprite4.y = 0;
    p->posSprite4.w = p->jump->w /4; 
    p->posSprite4.h = p->jump->w ; 
    
    p->posSprite5.x = 0;
    p->posSprite5.y = 0;
    p->posSprite5.w = p->jumpleft->w /4; 
    p->posSprite5.h = p->jumpleft->w ; 
   
   p->right=0;
   p->left=0;
   p->upm=0;
   p->manette=1;
   p->fight=1;
}



void afficherPerso(Personne p, SDL_Surface * screen) {
if(p.current_sprite == p.idle)
    SDL_BlitSurface(p.current_sprite, &p.posSprite, screen, &p.posScreen);

   if (p.current_sprite == p.run)
    SDL_BlitSurface(p.current_sprite, &p.posSprite2, screen, &p.posScreen);  
   if (p.current_sprite == p.runleft)
   SDL_BlitSurface(p.current_sprite, &p.posSprite3, screen, &p.posScreen);
   
    if (p.current_sprite == p.jump)
        SDL_BlitSurface(p.current_sprite, &p.posSprite4, screen, &p.posScreen);
     if (p.current_sprite == p.jumpleft)
        SDL_BlitSurface(p.current_sprite, &p.posSprite5, screen, &p.posScreen);    
   
}


void moveperso(Personne *p)
{
    if(p->move == 1)
    {

        p->posScreen.x += p->direction *p->speed;
        if(p->posScreen.x > SCREEN_W-60 || p->posScreen.x < 0)
        {
            p->direction = 0;
            p->move = 0;
        }
    }

}

void saut(Personne* p, int *flag) {
    if (*flag) {
        if (!p->isJumping) { 
            if (p->direction == 1) {
                p->current_sprite = p->jump;
            } else if (p->direction == -1) {
                p->current_sprite = p->jumpleft; 
            }
            p->isJumping = 1; 
            p->jumpSpeed = JUMP_SPEED;
        }
        *flag = 0;
    }

    if (p->isJumping) {
        p->posScreen.y -= p->jumpSpeed;
        p->jumpSpeed -= GRAVITY;

        if (p->posScreen.y >= p->ground) {
            p->isJumping = 0; 
            p->posScreen.y = p->ground; 
        }
    }
}
void animerperso(Personne* p) {


    if (p->posSprite.x >= p->current_sprite->w - p->posSprite.w) {
        p->posSprite.x = 0; 
    } else  {
        p->posSprite.x = p->posSprite.x + p->posSprite.w; 
    } 
    
    if( p->direction == 1)
    {
    

    if (p->posSprite2.x >= p->current_sprite->w - p->posSprite2.w) {
        p->posSprite2.x = 0; 
    } else  {
        p->posSprite2.x = p->posSprite2.x + p->posSprite2.w; 
    } 
    }
     else if (p->direction == -1) {
        if (p->posSprite3.x <= 0) {
            p->posSprite3.x = p->current_sprite->w - p->posSprite3.w; 
        } else {
            p->posSprite3.x = p->posSprite3.x - p->posSprite3.w; 
        }
        }
        
       if (p->current_sprite == p->jump) {
       if (p->direction == 1) {
    if (p->posSprite4.x >= p->current_sprite->w - p->posSprite4.w) {
        p->posSprite4.x = 0;
    } else {
        p->posSprite4.x += p->posSprite4.w; 
    }
} else if (p->direction == -1) {
    if (p->posSprite5.x >= p->current_sprite->w - p->posSprite5.w) {
        p->posSprite5.x = 0;
    } else {
        p->posSprite5.x += p->posSprite5.w; 
    }
}
        }
        
        

}





