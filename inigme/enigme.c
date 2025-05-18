#include "enigme.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
void initanim(enigme *e){
    e->anim[0] =IMG_Load("corr1.png");
    e->anim[1] =IMG_Load("corr2.png");
    e->anim[2] =IMG_Load("corr3.png");
    e->bgenigme = IMG_Load("bgc2.jpg");
}
void afficherEnigme(enigme e, SDL_Surface *screen) {
    SDL_Surface *image = IMG_Load("bgc2.jpg");
    if (image == NULL) {
        printf("Erreur de chargement de l'image : %s\n", SDL_GetError());
        return;
    }
    
   
    SDL_BlitSurface(image, NULL, screen, NULL);

    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return;
    }

    TTF_Font *font = TTF_OpenFont("pixelmix.ttf", 16);
    if (font == NULL) {
        printf("Erreur de chargement de la police de caractères : %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255}; 
    SDL_Surface *questionSurface = TTF_RenderText_Solid(font, e.question, textColor);
    SDL_Rect questionRect = {110, 160}; 
    SDL_BlitSurface(questionSurface, NULL, screen, &questionRect);
    SDL_FreeSurface(questionSurface);

    for (int i = 0; i < 3; ++i) {
        SDL_Surface *reponseSurface = TTF_RenderText_Solid(font, e.reponses[i], textColor);
        SDL_Rect reponseRect = {130, 210 + i * 50};
        SDL_BlitSurface(reponseSurface, NULL, screen, &reponseRect);
        SDL_FreeSurface(reponseSurface);
    }

    SDL_Flip(screen);

 

    TTF_CloseFont(font);
    TTF_Quit();
}
void InitEnigme(enigme *e, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
        return;
    }

    fgets(e->question, sizeof(e->question), fichier);

    for (int i = 0; i < 3; ++i) {
        fgets(e->reponses[i], sizeof(e->reponses[i]), fichier);
    }

    fclose(fichier);
    

    
}
void animerEnig(enigme *e, SDL_Surface *screen) {
    static int frame = 0;
    static int lastFrameChange = 0;
    const int FRAME_DURATION = 350; // milliseconds

    int currentTime = SDL_GetTicks();
    if (currentTime - lastFrameChange >= FRAME_DURATION) {
        frame = (frame + 1) % 3; // Assuming 3 frames in the animation
        lastFrameChange = currentTime;
    }

    SDL_Rect destRect = {100, 200, 0, 0}; // Adjust this rect as needed
    SDL_BlitSurface(e->bgenigme, NULL, screen, NULL);
    SDL_BlitSurface(e->anim[frame], NULL, screen, &destRect);
    
    SDL_Flip(screen); // Update the screen
}

