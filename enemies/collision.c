#include "collision.h"
#define INITIAL_M2_X 500
#define INITIAL_M2_Y 200
#define MOVE_LEFT   1
#define MOVE_RIGHT  2
#define MOVE_UP     4
#define MOVE_DOWN   8
#include <math.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

void initBack(Background *b){
    b->image_back = IMG_Load("back.png");
    if (b->image_back == NULL) {
        fprintf(stderr, "Failed to load background image: %s\n", IMG_GetError());
        // Handle the error (e.g., return or exit)
        return;
    }

    b->pos_ecran_back.x = 0;
    b->pos_ecran_back.y = 0;
    b->pos_ecran_back.w = b->image_back->w;
    b->pos_ecran_back.h = b->image_back->h;
}

void inite(Entity *e,SDL_Surface* ecran){

e->image_m1 = IMG_Load("m1.png");
   

    e->pos_ecran_m1.x = 100;
    e->pos_ecran_m1.y = 200;

    e->image_m2[0] = IMG_Load("drone-1.png");
    e->image_m2[1] = IMG_Load("drone-2.png");
    e->image_m2[2] = IMG_Load("drone-3.png");
    e->image_m2[3] = IMG_Load("drone-4.png");
    

    e->pos_ecran_m2.x = INITIAL_M2_X;
    e->pos_ecran_m2.y = INITIAL_M2_Y;

    e->image_h1 = IMG_Load("h1.png");
    

    e->image_h2 = IMG_Load("h2.png");
    

    e->image_h3 = IMG_Load("h3.png");
    

    e->image_h4 = IMG_Load("h4.png");
    
    e->image_c1 = IMG_Load("c1.png");
   
    e->pos_ecran_h1.x = 0;
    e->pos_ecran_h1.y = 0;
    e->pos_ecran_h1.w = e->image_m1->w;
    e->pos_ecran_h1.h = e->image_m1->h;
    
   e->image_gov = IMG_Load("gov.png");
   TTF_Init();
   e->font = TTF_OpenFont("arial.ttf", 24);
   
    e->textColor.r = 255;
    e->textColor.g = 255;
    e->textColor.b = 255;
    e->movement_flags = 0;
    e->isGameOver = 0;
    e->score = 0;
    e->touch_counter = 0;
    e->last_touch_time = 0;
    e->is_c1_hidden = 0;
    e->c1_visible = 1;
    e->bounce_direction = -1;
    e->returning_to_initial = 0;
    e->last_score_update_time = SDL_GetTicks();
    e->last_blink_time = SDL_GetTicks();
}

void afficherEnnemi(Entity e, SDL_Surface  * screen){
    int start = SDL_GetTicks();
    static int end = 0;
    static int x = 0;
    if (start - end >= 350) {

        x = (x+1) % 4;
        end = start;
    }

SDL_BlitSurface(e.image_m2[x], NULL, screen, &e.pos_ecran_m2);

}

void moveIA(Entity *e) {
 // Calculate the distance between image_m1 and image_m2
int dx = e->pos_ecran_m2.x - e->pos_ecran_m1.x;
int dy = e->pos_ecran_m2.y - e->pos_ecran_m1.y;
 e->distance = sqrt(dx * dx + dy * dy);

// If the distance between image_m1 and image_m2 is less than 70 pixels, adjust the position of image_m2
if (e->distance < 50) {
    // Calculate the distance needed to maintain a separation of 70 pixels
    int separationDistance = 50 - e->distance;

    // Calculate the unit vector from image_m2 to image_m1
    float magnitude = sqrt(dx * dx + dy * dy);
    float unitX = dx / magnitude;
    float unitY = dy / magnitude;

    // Move image_m2 to maintain a distance of 70 pixels from image_m1
    e->pos_ecran_m2.x += separationDistance * unitX;
    e->pos_ecran_m2.y += separationDistance * unitY;
}



  
}

SDL_Rect generateRandomPosition(int min_x, int max_x, int min_y, int max_y) {
    SDL_Rect random_pos;
    random_pos.x = min_x + rand() % (max_x - min_x + 1);
    random_pos.y = min_y + rand() % (max_y - min_y + 1);
    return random_pos;
}
void collisionTri( Entity *e,SDL_Surface  * ecran){
do {

        e->pos_ecran_c1 = generateRandomPosition(0, ecran->w - e->image_c1->w, 0, ecran->h - e->image_c1->h);
    } while ((abs(e->pos_ecran_c1.x - e->pos_ecran_m1.x) < e->image_m1->w && abs(e->pos_ecran_c1.y - e->pos_ecran_m1.y) < e->image_m1->h) ||
             (abs(e->pos_ecran_c1.x - e->pos_ecran_m2.x) < e->image_m2[0]->w && abs(e->pos_ecran_c1.y - e->pos_ecran_m2.y) < e->image_m2[0]->h));
}


void move( Entity * e){
if (e->movement_flags & MOVE_LEFT) {
                e->pos_ecran_m1.x -= 5;
            }
            if (e->movement_flags & MOVE_RIGHT) {
                e->pos_ecran_m1.x += 5;
            }
            if (e->movement_flags & MOVE_UP) {
                e->pos_ecran_m1.y -= 5;
            }
            if (e->movement_flags & MOVE_DOWN) {
                e->pos_ecran_m1.y += 5;
            }
}

void collisionBB( Entity e,SDL_Surface  * ecran,Background b2){
 
 SDL_BlitSurface(e.image_m1, NULL, ecran, &e.pos_ecran_m1);
 if (e.touch_counter == 0) {
            SDL_BlitSurface(e.image_h1, NULL, ecran, &e.pos_ecran_h1);
        } else if (e.touch_counter == 1) {
            SDL_BlitSurface(e.image_h2, NULL, ecran, &e.pos_ecran_h1);
        } else if (e.touch_counter == 2) {
            SDL_BlitSurface(e.image_h3, NULL, ecran, &e.pos_ecran_h1);
        } else {
            SDL_BlitSurface(e.image_h4, NULL, ecran, &e.pos_ecran_h1);
        }
if (!e.isGameOver) {
           
            sprintf(e.score_text, "Score: %d", e.score);
            SDL_Surface* score_surface = TTF_RenderText_Solid(e.font, e.score_text, e.textColor);
           
            SDL_Rect pos_ecran_score = {ecran->w - score_surface->w - 10, 10, 0, 0};
            SDL_BlitSurface(score_surface, NULL, ecran, &pos_ecran_score);
            SDL_FreeSurface(score_surface);
        }

        // Blit c1.png onto the screen if it's not hidden and visible
        if (!e.is_c1_hidden && e.c1_visible) {
            SDL_BlitSurface(e.image_c1, NULL, ecran, &e.pos_ecran_c1);
        }
}

void FreeIMG( Entity e , Background b2){
    SDL_FreeSurface(b2.image_back);
    SDL_FreeSurface(e.image_m1);
    for (int i = 0; i < 5; i++) {
    SDL_FreeSurface(e.image_m2[i]);
}
    SDL_FreeSurface(e.image_h1);
    SDL_FreeSurface(e.image_h2);
    SDL_FreeSurface(e.image_h3);
    SDL_FreeSurface(e.image_h4);
    SDL_FreeSurface(e.image_gov);
    SDL_FreeSurface(e.image_c1);
    TTF_CloseFont(e.font);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
}
