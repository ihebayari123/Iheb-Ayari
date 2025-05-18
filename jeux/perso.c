#include "perso.h"
#include <SDL/SDL_image.h>
#include <stdbool.h>
#define GRAVITY 3
#define JUMP_SPEED 25 // Adjust as needed
#define MOVE_SPEED 1 // Adjust as needed
#define JUMP_FORCE 10 // Adjust as needed




void initPerso(Personne *p) {
        p->idle = IMG_Load("perso animation/Punk_idle.png");
        p->run = IMG_Load("perso animation/Punk_run.png");
        p->runleft = IMG_Load("perso animation/Punk_runleft.png");
        p->jump = IMG_Load("perso animation/Punk_jump.png");
        p->jumpleft = IMG_Load("perso animation/Punk_jumpleft.png");
        p->doublejump = IMG_Load("perso animation/Punk_doublejump right.png");
        p->doublejumpleft = IMG_Load("perso animation/Punk_doublejump left.png");
    
        p->idle2 = IMG_Load("perso animation/Cyborg_idle.png");
        p->run2 = IMG_Load("perso animation/Cyborg_run.png");
        p->runleft2 = IMG_Load("perso animation/Cyborg_runleft.png");
        p->jump2 = IMG_Load("perso animation/Cyborg_jump.png");
        p->jumpleft2 = IMG_Load("perso animation/Cyborg_jumpleft.png");
        p->doublejump2 = IMG_Load("perso animation/Cyborg_doublejump.png");
        p->doublejumpleft2 = IMG_Load("perso animation/Cyborg_doublejumpleft.png");
  
   
    
    p->current_sprite = p->idle;
    p->current_sprite2 = p->idle2;
   
    p->speed=0;
    
    p->ground = 360;
     if (p->idle == NULL) {
        printf("Unable to load idle image! SDL_Error: %s\n", SDL_GetError());

        return;
    }
    //punk
    p->direction = 0;
    p->posScreen.x = 0;
    p->posScreen.y = 0;
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
    
    p->posSprite6.x = 0;
    p->posSprite6.y = 0;
    p->posSprite6.w = p->doublejump->w /6; 
    p->posSprite6.h = p->doublejump->w ; 
    
    p->posSprite7.x = 0;
    p->posSprite7.y = 0;
    p->posSprite7.w = p->doublejumpleft->w /6; 
    p->posSprite7.h = p->doublejumpleft->w ; 
    
  // cyborg  
    p->posSpritec.x = 0;
    p->posSpritec.y = 0;
    p->posSpritec.w = p->idle2->w /4; 
    p->posSpritec.h = p->idle2->w ; 
    
    p->posSprite2c.x = 0;
    p->posSprite2c.y = 0;
    p->posSprite2c.w = p->run2->w /6; 
    p->posSprite2c.h = p->run2->w ; 
    
    p->posSprite3c.x = 0;
    p->posSprite3c.y = 0;
    p->posSprite3c.w = p->runleft2->w /6; 
    p->posSprite3c.h = p->runleft2->w ; 
    
    p->posSprite4c.x = 0;
    p->posSprite4c.y = 0;
    p->posSprite4c.w = p->jump2->w /4; 
    p->posSprite4c.h = p->jump2->w ; 
    
    p->posSprite5c.x = 0;
    p->posSprite5c.y = 0;
    p->posSprite5c.w = p->jumpleft2->w /4; 
    p->posSprite5c.h = p->jumpleft2->w ; 
    
    p->posSprite6c.x = 0;
    p->posSprite6c.y = 0;
    p->posSprite6c.w = p->doublejump2->w /6; 
    p->posSprite6c.h = p->doublejump2->w ; 
    
    p->posSprite7c.x = 0;
    p->posSprite7c.y = 0;
    p->posSprite7c.w = p->doublejumpleft2->w /6; 
    p->posSprite7c.h = p->doublejumpleft2->w ; 
    
    
    
     p->jumpCount = 0;
     
     p->numperso == 0;
}



void afficherPerso(Personne p, SDL_Surface * screen,int choix) {
//punk
if (choix == 1){
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
     if (p.current_sprite == p.doublejump)
        SDL_BlitSurface(p.current_sprite, &p.posSprite6, screen, &p.posScreen);   
     if (p.current_sprite == p.doublejumpleft)
        SDL_BlitSurface(p.current_sprite, &p.posSprite7, screen, &p.posScreen);  
}
//cyborg
else if(choix == 2){
if(p.current_sprite2 == p.idle2)
    SDL_BlitSurface(p.current_sprite2, &p.posSpritec, screen, &p.posScreen);

   if (p.current_sprite2 == p.run2)
    SDL_BlitSurface(p.current_sprite2, &p.posSprite2c, screen, &p.posScreen);  
   if (p.current_sprite2 == p.runleft2)
   SDL_BlitSurface(p.current_sprite2, &p.posSprite3c, screen, &p.posScreen);
   
    if (p.current_sprite2 == p.jump2)
        SDL_BlitSurface(p.current_sprite2, &p.posSprite4c, screen, &p.posScreen);
     if (p.current_sprite2 == p.jumpleft2)
        SDL_BlitSurface(p.current_sprite2, &p.posSprite5c, screen, &p.posScreen);   
     if (p.current_sprite2 == p.doublejump2)
        SDL_BlitSurface(p.current_sprite2, &p.posSprite6c, screen, &p.posScreen);   
     if (p.current_sprite2 == p.doublejumpleft2)
        SDL_BlitSurface(p.current_sprite2, &p.posSprite7c, screen, &p.posScreen); 
               
}
}



void moveperso(Personne *p, Background *bg, SDL_Surface* mask) {
    if (p->move == 1) {
        p->posScreen.x += p->direction * p->speed;

        // Adjust character's position to stay within the background boundaries
        if (p->posScreen.x < 0) {
            p->posScreen.x = 0;
        } else if (p->posScreen.x > (1080)) {
            p->posScreen.x =1080;
        }
        
        if (collisionPP(p->posScreen, mask)) {
            // If collision detected, adjust character's y position to stay on the ground
            p->posScreen.y -= GRAVITY; // Adjust GRAVITY as needed
        }
    }
}

void saut(Personne* p, int *flag,int choix) {
  
    static int jumpHoldTimer = 0;
    
    if (choix == 1){
    if (*flag) {
        if (!p->isJumping || p->jumpCount < 2) { 
            
                if (p->direction == 1) {
                    p->current_sprite = p->jump;
                    if (p->jumpCount == 1)
                    p->current_sprite = p->doublejump;
                } else if (p->direction == -1) {
                    p->current_sprite = p->jumpleft; 
                    if (p->jumpCount == 1)
                     p->current_sprite = p->doublejumpleft;
                    
                }
                
                
                p->isJumping = 1; 
                p->jumpSpeed = JUMP_SPEED;
                p->jumpCount++; 
                jumpHoldTimer = 2; 
            
        }
        *flag = 0;
    }

    if (p->isJumping) {
        if (p->jumpSpeed <= 0) {
           
            p->posScreen.y += GRAVITY;
        } else {
           
            p->posScreen.y -= p->jumpSpeed;
            p->jumpSpeed -= GRAVITY;

            if (p->jumpSpeed <= 0 && jumpHoldTimer > 0) {
               
                jumpHoldTimer--;
            }
        }

        
        if (p->posScreen.y >= p->ground) {
            p->isJumping = 0; 
            p->posScreen.y = p->ground; 
             if (p->direction == 1)
             p->current_sprite = p->run;
              if (p->direction == -1)
              p->current_sprite = p->runleft;
              if (p->direction == 0)
            p->current_sprite = p->idle;
            p->jumpCount = 0; 
        } else if (jumpHoldTimer == 0) {
           
            p->posScreen.y += GRAVITY;
        }
    }
    
    }
    else if (choix == 2){
    if (*flag) {
        if (!p->isJumping || p->jumpCount < 2) { 
            
                if (p->direction == 1) {
                    p->current_sprite2 = p->jump2;
                    if (p->jumpCount == 1)
                    p->current_sprite2 = p->doublejump2;
                } else if (p->direction == -1) {
                    p->current_sprite2 = p->jumpleft2; 
                    if (p->jumpCount == 1)
                     p->current_sprite2 = p->doublejumpleft2;
                    
                }
                
                
                p->isJumping = 1; 
                p->jumpSpeed = JUMP_SPEED;
                p->jumpCount++; 
                jumpHoldTimer = 2; 
            
        }
        *flag = 0;
    }

    if (p->isJumping) {
        if (p->jumpSpeed <= 0) {
           
            p->posScreen.y += GRAVITY;
        } else {
           
            p->posScreen.y -= p->jumpSpeed;
            p->jumpSpeed -= GRAVITY;

            if (p->jumpSpeed <= 0 && jumpHoldTimer > 0) {
               
                jumpHoldTimer--;
            }
        }

        
        if (p->posScreen.y >= p->ground) {
            p->isJumping = 0; 
            p->posScreen.y = p->ground; 
             if (p->direction == 1)
             p->current_sprite2 = p->run2;
              if (p->direction == -1)
              p->current_sprite2 = p->runleft2;
              if (p->direction == 0)
            p->current_sprite2 = p->idle2;
            p->jumpCount = 0; 
        } else if (jumpHoldTimer == 0) {
           
            p->posScreen.y += GRAVITY;
        }
    }
    
    }
    
   
   
}



void animerperso(Personne* p,int choix) {
if (choix == 1){
    static int animationCounter = 0;
    int animationThreshold = 10;

    animationCounter++;
    if (animationCounter >= animationThreshold) {
        animationCounter = 0;
        if (p->posSprite.x >= p->current_sprite->w - p->posSprite.w) {
            p->posSprite.x = 0;
        } else {
            p->posSprite.x = p->posSprite.x + p->posSprite.w;
        }

        if (p->direction == 1) {
            if (p->posSprite2.x >= p->current_sprite->w - p->posSprite2.w) {
                p->posSprite2.x = 0;
            } else {
                p->posSprite2.x = p->posSprite2.x + p->posSprite2.w;
            }
        } else if (p->direction == -1) {
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

        if (p->current_sprite == p->doublejump || p->current_sprite == p->doublejumpleft) {
            if (p->direction == 1) {
                if (p->posSprite6.x >= p->current_sprite->w - p->posSprite6.w) {
                    p->posSprite6.x = 0;
                } else {
                    p->posSprite6.x += p->posSprite6.w;
                }
            } else if (p->direction == -1) {
                if (p->posSprite7.x >= p->current_sprite->w - p->posSprite7.w) {
                    p->posSprite7.x = 0;
                } else {
                    p->posSprite7.x += p->posSprite7.w;
                }
            }
        }
    }
   }
  else if (choix == 2){
  
  static int animationCounter2 = 0;
    int animationThreshold2 = 10;

    animationCounter2++;
    if (animationCounter2 >= animationThreshold2) {
        animationCounter2 = 0;
        if (p->posSpritec.x >= p->current_sprite2->w - p->posSpritec.w) {
            p->posSpritec.x = 0;
        } else {
            p->posSpritec.x = p->posSpritec.x + p->posSpritec.w;
        }

        if (p->direction == 1) {
            if (p->posSprite2c.x >= p->current_sprite2->w - p->posSprite2c.w) {
                p->posSprite2c.x = 0;
            } else {
                p->posSprite2c.x = p->posSprite2c.x + p->posSprite2c.w;
            }
        } else if (p->direction == -1) {
            if (p->posSprite3c.x <= 0) {
                p->posSprite3c.x = p->current_sprite2->w - p->posSprite3c.w;
            } else {
                p->posSprite3c.x = p->posSprite3c.x - p->posSprite3c.w;
            }
        }

        if (p->current_sprite2 == p->jump2) {
            if (p->direction == 1) {
                if (p->posSprite4c.x >= p->current_sprite2->w - p->posSprite4c.w) {
                    p->posSprite4c.x = 0;
                } else {
                    p->posSprite4c.x += p->posSprite4c.w;
                }
            } else if (p->direction == -1) {
                if (p->posSprite5c.x >= p->current_sprite2->w - p->posSprite5c.w) {
                    p->posSprite5c.x = 0;
                } else {
                    p->posSprite5c.x += p->posSprite5c.w;
                }
            }
        }

        if (p->current_sprite2 == p->doublejump2 || p->current_sprite2 == p->doublejumpleft2) {
            if (p->direction == 1) {
                if (p->posSprite6c.x >= p->current_sprite2->w - p->posSprite6c.w) {
                    p->posSprite6c.x = 0;
                } else {
                    p->posSprite6c.x += p->posSprite6c.w;
                }
            } else if (p->direction == -1) {
                if (p->posSprite7c.x >= p->current_sprite2->w - p->posSprite7c.w) {
                    p->posSprite7c.x = 0;
                } else {
                    p->posSprite7c.x += p->posSprite7c.w;
                }
            }
        }
    }
  
  } 
   
}





