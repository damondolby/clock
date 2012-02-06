#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include "clockface.h"
# include <math.h>

int Running;
SDL_Surface *screen;
TTF_Font* font;
int NewTime;
char TimeToDisplay[5];
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
	NewTime = 1;
	
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
					printf("1. mouse down\n");
					handle_mouse_down(mouse_x, mouse_y);
					printf("2. mouse down\n");
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
		
		//todo: time_t is a restricted type, and 
		//may not be meaningfully converted to unsigned int 
		//on some systems?
		srand ( (unsigned int)time ( NULL ) ); 
		h = getHour();
		m = getMinute();
		len = hourMinToStr(h, m, TimeToDisplay);
	}
	
	//check hand collision
	
}

void draw_rectangle()
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 50;
	rect.w = scr_wid;
	rect.h = 50;
	Uint32 color = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	SDL_FillRect(screen, &rect, color);
}

void draw_background()
{
	SDL_Rect rect;
	rect.x = 10;
	rect.y = 10;
	rect.w = 20;
	rect.h = 20;
	Uint32 color = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	SDL_FillRect(screen, &screen->clip_rect, color);
	//DL_BlitSurface(textSurface, NULL, screen, &textLocation);
	//SDL_Flip(screen);
}

int render() {
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Press return...", foregroundColor, backgroundColor);
	draw_rectangle();
	// Pass zero for width and height to draw the whole surface 
	SDL_Rect textLocation = { scr_wid/2-20, 50, 0, 0 };
	
	/*SDL_Rect rect;
	rect.x = 10;
	rect.y = 10;
	rect.w = 20;
	rect.h = 20;
	Uint32 color = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);*/
	//SDL_FillRect(screen, &screen->clip_rect, color);
	
	
	SDL_Color foregroundColor = { 255, 255, 255 }; 
	SDL_Color backgroundColor = { 0, 0, 255 };		
	
	//clear the screen
	//SDL_FillRect(screen, &textLocation, 0);
	//SDL_Flip(screen);

	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, TimeToDisplay, foregroundColor, backgroundColor);
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, TimeToDisplay, foregroundColor);
	
	//draw_background();
	//draw_rectangle();
	
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
	//x = get_random();
	//x = srand ( (unsigned int)time ( NULL ) );
	//x = srand(100);
	printf("x=%d\n", x);
	m = x % 59;
	
	//Calculate remainder and round to nearest 5
	//r = m % 10;
	
	//printf("random: %d, min: %d, test: %d, test2: %d, test3: %d\n", x, m, m % 10, m % 1, m/10);	
	return round5(m);
}

/*int get_random()
{
	http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
	int r, low, high;
	r = -1;
	low = 0;
	high = 59;
 
	while ( r < low || high < r )
	{
	  r = rand();
	}
}*/

 /*unsigned time_seed()
 {
 time_t now = time ( 0 );
 unsigned char *p = (unsigned char *)&now;
 unsigned seed = 0;
 size_t i;
 
 for ( i = 0; i < sizeof now; i++ )
 seed = seed * ( UCHAR_MAX + 2U ) + p[i];

return seed;
}*/

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
	
	//s[i] = '\0';
	if (i == 4)
		s[4] = 0;
		//s[4] = '\0';
	printf("0=%d, 1=%d, 2=%d, 3=%d, 4=%d, 5=%d, i=%d, time: %s\n", s[0] - '0', s[1] - '0', s[2] - '0', s[3] - '0', s[4] - '0', s[5] - '0', i, s);
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
