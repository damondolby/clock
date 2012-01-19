#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include "clockface.h"
# include <math.h>

int Running;
SDL_Surface *screen;
TTF_Font* font;
int NewTime;
char TimeToDisplay[6];
int scr_wid = 640;
int scr_hi = 480;
int mouse_x;
int mouse_y;

//void writeText2(char* txt, int x, int y);

int main() {
	
	//gcc clock.c clockface.c -lSDL -lSDL_ttf -o clock.out

	init();
	
	run();
	
	cleanup();	
	
	return 0;
}

int init() {
	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
										
	screen = SDL_SetVideoMode(scr_wid, scr_hi, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set %ix%i video: %s\n", scr_wid, scr_hi, SDL_GetError());
		exit(1);
	}	
										
	printf("Set window of %ix%i at %d bits per pixel mode\n", scr_wid, scr_hi,
	screen->format->BitsPerPixel);
	TTF_Init();      
	
	font = TTF_OpenFont("FreeSans.ttf", 26);
	
	Running = 1;
	NewTime = 0;
	
	draw_clock_face(screen, scr_wid, scr_hi);
	
	//writeText2("hey", 10, 10);
	//writeText2("hey2222", 100, 100);
}

int events(SDL_Event *event) {
	
	switch( event->type ){
		case SDL_QUIT:
			Running = 0;
			break;
		case SDL_KEYDOWN:			
			switch (event->key.keysym.sym) 
			{					
			   case SDLK_RETURN:
				   NewTime = 1;
				break;					
			   case SDLK_q:
				Running = 0;
				break;
		      }
		      break;
		case SDL_KEYUP:				      
			break;
		
		case SDL_MOUSEBUTTONDOWN: 
			switch(event->button.button) {
				case SDL_BUTTON_LEFT: 
					mouse_x = event->motion.x;
					mouse_y = event->motion.y;
					//printf("left pressed. x:%d, y:%d\n", event->motion.x, event->motion.y);
					//OnLButtonDown(Event->button.x,Event->button.y);
					
					//if (hand_collision(mouse_x, mouse_y))
					//{
					//	printf("yes!\n");
					//}
					handle_mouse_down(mouse_x, mouse_y);
					
					break;
				
				case SDL_BUTTON_RIGHT: 
					//printf("right pressed");
					//OnRButtonDown(Event->button.x,Event->button.y);
					break;
				
				case SDL_BUTTON_MIDDLE: 
				    //OnMButtonDown(Event->button.x,Event->button.y);
				    break;						
			}
			break;
			
		case SDL_MOUSEBUTTONUP: 
			
			switch(event->button.button) {
				case SDL_BUTTON_LEFT:
					
					mouse_x = event->motion.x;
					mouse_y = event->motion.y;
				
					handle_mouse_up(screen, mouse_x, mouse_y, scr_wid, scr_hi);
					
					break;			
			}
			break;
	}

}

int run() {
	
	SDL_Event event;
	
	while (Running) {
		
		while (SDL_PollEvent(&event))
		{
			events(&event);	
			loop();
			render();
		}
	}
}

int loop() {	
	
	if (NewTime)
	{		
		int h;
		int m;
		int len;
				
		NewTime = 0;
		
		h = getHour();
		m = getMinute();
		len = hourMinToStr(h, m, TimeToDisplay);
	}
	
	//check hand collision
	
}

int render() {
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Press return...", foregroundColor, backgroundColor);

	// Pass zero for width and height to draw the whole surface 
	SDL_Rect textLocation = { scr_wid/2-20, 50, 0, 0 };
	
	SDL_Color foregroundColor = { 255, 255, 255 }; 
	SDL_Color backgroundColor = { 0, 0, 255 };		

	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, TimeToDisplay, foregroundColor, backgroundColor);
	
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0,0));
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
	
	//draw_circle(screen);
}

int cleanup() {
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

int getHour()
{
	int x;
	x = rand(); // everytime it is different because the seed is different.
	return (x % 12) + 1;
}

int getMinute()
{
	int x, m, r;
	char s;
	x = rand(); // everytime it is different because the seed is different.
	m = x % 59;
	
	//Calculate remainder and round to nearest 5
	//r = m % 10;
	
	//printf("random: %d, min: %d, test: %d, test2: %d, test3: %d\n", x, m, m % 10, m % 1, m/10);	
	return round5(m);
}

int round5(int m)
{
	int x, y;
	x = m % 10; //remainder
	y = m / 10; //10th
	
	if (x >= 8)
	{
		//Round up to nearest 10 (e.g. 38, 39 will round to 40)
		y++;
		x = 0;
	}
	else if (x <= 7 && x >= 3)
	{
		//Round to nearest 5 (e.g. 33, 34, 35, 36, 37 will round to 35)
		x = 5;
	}
	else
		x = 0; //Round down to nearest 10 (e.g. 30, 31, 32 will round to 30)
	
	return (y*10)+x; //TODO: round to 5
}

int hourMinToStr(int h, int m, char* s)
{
	int i = 0;
	
	if (h >= 10)
	{
		s[i++] = (h/10) % 10 + '0';
		s[i++] = h%10 + '0';
	}	
	else			
	{
		s[i++] = h + '0';
	}
	
	s[i++] = ':';	
	
	if (m < 10)
	{
		s[i++] = '0';		
	}
	else
	{
		s[i++] = (m/10) % 10 + '0';
	}
	s[i++] = m%10 + '0';
	s[i] = '\0';
	return i;
}



/*void writeText2(char* txt, int x, int y)
{	
	SDL_Rect textLocation = { x, y, 0, 0 };
	
	SDL_Rect textLocation2 = { x+50, y+50, 0, 0 };
	
	SDL_Color foregroundColor = { 255, 255, 0 }; 
	SDL_Color backgroundColor = { 0, 0, 255 };
	
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "This is mytext.", foregroundColor, backgroundColor);
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, txt, foregroundColor, backgroundColor);
	SDL_Surface* textSurface2 = TTF_RenderText_Shaded(font, "text", foregroundColor, backgroundColor);

	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 0,0));
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_BlitSurface(textSurface2, NULL, screen, &textLocation2);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);
	SDL_FreeSurface(textSurface2);
	
}
*/
