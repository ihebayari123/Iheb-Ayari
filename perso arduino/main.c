#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "perso.h"

#define SCREEN_H 800
#define SCREEN_W 1200
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "serie.h"

int main() {
//--------------arduino
	// arduino
	//  Open the serial port
	
	struct termios options;
	char buffer[100];
	int fd = serialport_init("/dev/ttyUSB0", 9600);

    if (fd == -1) {
    printf("gggg");
        return -1;

    }
	fcntl(fd, F_SETFL, 0);
	tcgetattr(fd, &options);
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(fd, TCSANOW, &options);
	tcflush(fd, TCIOFLUSH);
	//
	//-------------arduino
    int flag = 0; // Initialize flag for key release event
    int jumpFlag = 0;
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE);
    if (screen == NULL) {
        printf("Screen surface could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    Personne perso;
    initPerso(&perso);

    SDL_Surface* image = IMG_Load("background.jpg");
    if (image == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Rect imagepos;

    imagepos.x = 0;
    imagepos.y = 0;
    int ctrl_pressed = 0;
    int quit = 0;
    SDL_Event event;
    int i;
    while (!quit) {
    serialport_read_until(fd,buffer,'\n',99,10);
      for(i=0;buffer[i]!='\r' && i<100;i++);
      buffer[i]=0;
      printf("%s",buffer);
      if(strstr((buffer),"right")){

                            perso.speed = 5;
                            perso.move = 1;
                            perso.direction =1;
              perso.current_sprite =perso.run;
      }
      if(strstr((buffer),"left")){
                            perso.speed = 5;
                            perso.move = 1;
                             perso.direction =-1;
              perso.current_sprite =perso.runleft;
      }
      if(strstr((buffer),"JUMP")){
        flag = 1;
      }
      if(strstr((buffer),"Acceleration")){
        perso.speed = 20; 
      }
      /*if(strstr((buffer),"")){
        k.RIGHT=0;
                    k.left=0;
      }*/
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                
             case SDL_KEYDOWN:
             
             if (event.key.keysym.sym == SDLK_RIGHT) {
             if (ctrl_pressed) {
                            perso.speed = 20;
                            perso.move = 1;
                
                if(perso.posScreen.x > SCREEN_W-60 )
                {
                 perso.move = 0;
                 }
                        } else {
                            perso.speed = 5;
                            perso.move = 1;
                
                if(perso.posScreen.x < 0)
                {
                 perso.move = 0;
                 }
                            }
              
              perso.direction =1;
              perso.current_sprite =perso.run;
             }
             if (event.key.keysym.sym == SDLK_LEFT) {
             if (ctrl_pressed) {
                            perso.speed = 20; 
                            perso.move = 1;
                
                if(perso.posScreen.x < 0)
                {
                 perso.move = 0;
                 }
                        } else {
                            perso.speed = 5;perso.move = 1;
                
                if(perso.posScreen.x < 0)
                {
                 perso.move = 0;
                 }
                 }
              
              perso.direction =-1;
              perso.current_sprite =perso.runleft;
             }
             if (event.key.keysym.sym == SDLK_SPACE) {
             
             
            
                            flag = 1;
                        
             }
             
             if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                 ctrl_pressed = 1;
                 }
             
             break;  
            case SDL_KEYUP:
            
             if (event.key.keysym.sym == SDLK_RIGHT) {
              perso.speed = 0;
              perso.direction =0;
              perso.move = 0;
              perso.current_sprite =perso.idle;
               }
               
             if (event.key.keysym.sym == SDLK_LEFT) {
              perso.speed = 0;
              perso.direction =0;
              perso.move = 0;
              perso.current_sprite =perso.idle;
               }  
               
             if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                 ctrl_pressed = 0;
                 }  
             if (event.key.keysym.sym == SDLK_SPACE) {
                 
                 flag = 0;
                 }
            break;
            }
        }
        //--------------arduino
		/*memset(&buffer, '\0', sizeof(buffer));
		if (read(fd, &buffer, sizeof(buffer)) > 0)
		{
			printf("Data read: %c\n", buffer);
			fflush(stdout);
		}
		switch (buffer)
		{
		case '7':
			// right on
			p.right = 1;
			break;
		case '9':
			// left on

			p.left = 1;
			break;
		case '3':
			p.upm = 1;
			// up on
			break;

		case '8':
			// left off
			p.left = 0;
			break;
		case '6':
			// right off
			p.right = 0;

			break;
		case '2':
			p.upm = 0;
			// up off
			break;

		case '4':
			p.fight = 1;
			// down on
			break;

		case '5':
			p.fight = 0;

			// down off
			break;
		}
if(p.right==1) {
                        p.direction = 0;
                        perso.speed = 5;
                        
}else if(p.right==0)  {
     p.direction = 2;
                        do{
                        perso.speed --;
                        }while(perso.speed ==0);
                        p.right=-1;
}

  if(p.left==1){
           p.direction = 1;
                        perso.speed = 5;
                        
}else if(p.left==0) {
          p.direction = 2;
                        perso.speed = 0;
                        p.left=-1;
                   
}
if (p.upm==1) {
     up1 = 1;
}else if (p.upm==0)  {    up1=0; p.upm=-1; }*/
		//--------------arduino
        
        SDL_BlitSurface(image,NULL,screen,&imagepos);
        
        moveperso(&perso);
        saut(&perso, &flag);
        
        animerperso(&perso);

        afficherPerso(perso, screen);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        
        SDL_Flip(screen);

       
        SDL_Delay(50);
    }

   
    SDL_FreeSurface(image);
    //SDL_FreeSurface(perso.idle);
    SDL_Quit();

    return 0;
}

