#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include "clock.h"
#include <math.h>

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
										
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set %ix%i video: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}	
										
	printf("Set window of %ix%i at %d bits per pixel mode\n", SCREEN_WIDTH, SCREEN_HEIGHT,
	screen->format->BitsPerPixel);
	TTF_Init();      
	
	font = TTF_OpenFont("FreeSans.ttf", 26);
	
	Running = 1;
	//NewTime = 1;
	
	//init_clock_face(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//generate_new_time();		
	
	//backgroundColor = { 64, 224, 208 };	
	
	backgroundColor = malloc(sizeof *backgroundColor);
	backgroundColor->r = 154;
	backgroundColor->g = 192;
	backgroundColor->b= 205;
	
	fontColor = malloc(sizeof *fontColor);
	fontColor->r = 16;
	fontColor->g = 78;
	fontColor->b= 139;
	
	blank_out_background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	render_welcome_screen();
	
	//16;78;139
	
	//writeText2("hey", 10, 10); = { 64, 224, 208 }
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
				   totalTimes++;
				   start_game();
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
					//printf("1. mouse down\n");
					handle_mouse_down(mouse_x, mouse_y);
					//printf("2. mouse down\n");
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
				
					handle_mouse_up(mouse_x, mouse_y);
				
					does_selection_match();
				
					//printf("Current hour: %d\n", get_current_hour());
					//printf("Current min: %d\n", get_current_min());
					
					
					
					break;			
			}
			break;
	}

}

int does_selection_match()
{
	int retVal = 0;
	
	if (generated_hour == get_selected_hour() &&
		generated_min == get_selected_min())
	{
		//printf("Time matches! gen hour: %d, sel hour: %d, gen min: %d, sel min: %d\n", 
		//	generated_hour, get_selected_hour(), generated_min, get_selected_min());
		totalTimes++;
		correctTimes++;
		make_background_flash();
		generate_new_time();
	}
	else
	{
		printf("Time does NOT match! gen hour: %d, sel hour: %d, gen min: %d, sel min: %d\n", 
			generated_hour, get_selected_hour(), generated_min, get_selected_min());
	}		
}

void make_background_flash()
{
	//blank_out_background(0, 100, SCREEN_WIDTH, 50, 0x00, 0x00, 0x80);
	blank_out_background((SCREEN_WIDTH/2)-75, 100, 150, 50, fontColor);
	render_correct_text();
	SDL_Delay(2000);
	//blank_out_background(0, 100, SCREEN_WIDTH, 50, 0xb0, 0xc4, 0xde);
	blank_out_background((SCREEN_WIDTH/2)-75, 100, 150, 50, backgroundColor);
}

int run() {
	
	SDL_Event event;
	//int workDone = 0;	
	
	while (Running) {
		
		//printf("running...\n");
		while (SDL_PollEvent(&event))
		{
			//workDone = 1;
			//printf("polling...\n");
			events(&event);	
			//loop();
			//render();
		}
		//if (workDone == 0)
		SDL_Delay(100);
		//SDL_WaitEvent();
			//sleep(1);
		//workDone = 0;
	}
}

int generate_new_time() {		
	//int h;
	//int m;
	int len;
			
	//NewTime = 0;
	
	//todo: time_t is a restricted type, and 
	//may not be meaningfully converted to unsigned int 
	//on some systems?
	srand ( (unsigned int)time ( NULL ) ); 
	generated_hour = getHour();
	generated_min = getMinute();
	len = hourMinToStr(generated_hour, generated_min, timeToDisplay);
	render_digital_clock();
	render_score();
	//check hand collision
	
}

void blank_out_background(int x, int y, int w, int h, SDL_Color *col)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	Uint32 color = SDL_MapRGB(screen->format, 
				col->r, 
				col->g, 
				col->b);
	SDL_FillRect(screen, &rect, color);
	SDL_Flip(screen);
}

void start_game()
{
	//blank_out_background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0xc4, 0xde);
	blank_out_background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	render_score();
	generate_new_time();
	init_clock_face(screen, SCREEN_WIDTH, SCREEN_HEIGHT);		
}

void render_digital_clock() {
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Press return...", foregroundColor, backgroundColor);
	//blank_out_background(0, 50, SCREEN_WIDTH, 50, 0x1e, 0x90, 0xff);
	blank_out_background(0, 50, SCREEN_WIDTH, 50, backgroundColor);	
	// Pass zero for width and height to draw the whole surface 
	SDL_Rect textLocation = { SCREEN_WIDTH/2-20, 50, 0, 0 };
	
	//SDL_Color foregroundColor = { 255, 255, 255 }; 
	//SDL_Color backgroundColor = { 0, 0, 255 };		
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, timeToDisplay, *fontColor);
	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
}

void render_score() {
	
	//blank_out_background(0, 0, SCREEN_WIDTH, 50, 0x1e, 0x90, 0xff);	
	blank_out_background(0, 0, SCREEN_WIDTH, 50, backgroundColor);
	
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Press return...", foregroundColor, backgroundColor);
	//blank_out_background(0, 50, SCREEN_WIDTH, 50);
	// Pass zero for width and height to draw the whole surface 
	SDL_Rect textLocation = { SCREEN_WIDTH-200, 20, 0, 0 };
	
	//SDL_Color foregroundColor = { 255, 255, 255 }; 
	//SDL_Color backgroundColor = { 0, 255, 255 };		
		
	//char* scoreStr;
	
	//Convert to function that can append strings and convert ints to string (i.e. itoa if I can figure it out!)
	char scoreStr[50];
	scoreStr[0] = 'S';
	scoreStr[1] = 'c';
	scoreStr[2] = 'o';
	scoreStr[3] = 'r';
	scoreStr[4] = 'e';
	scoreStr[5] = ':';
	scoreStr[6] = ' ';
	scoreStr[7] = correctTimes + '0'; //TODO: handle case when score might be more than 10!
	scoreStr[8] = ' ';
	scoreStr[9] = 'o';
	scoreStr[10] = 'f';
	scoreStr[11] = ' ';
	scoreStr[12] = totalTimes + '0';
	scoreStr[13] = '\0';
	//char* startStr = "Correct: ";
	char* totalStr;	
	//itoa(correct,totalStr);
	//char * scoreStr = malloc(snprintf(NULL, 0, "%s %s", startStr, totalStr) + 1);
	//scoreStr = "Correct: " + totalStr;
	
	//SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Correct: " + correct + " of " + total, foregroundColor);
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, scoreStr, *fontColor);
	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
}

void render_welcome_screen() {
	//blank_out_background(0, 0, SCREEN_WIDTH, 50);
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Press return...", foregroundColor, backgroundColor);
	//blank_out_background(0, 50, SCREEN_WIDTH, 50);
	// Pass zero for width and height to draw the whole surface 
	SDL_Rect textLocation = { 100, 200, 0, 0 };
	SDL_Rect textLocation2 = { 100, 230, 0, 0 };
	
	//SDL_Color foregroundColor = { 255, 255, 255 }; 
	//SDL_Color backgroundColor = { 0, 255, 255 };		
		
	char* welcomeStr;	
	char* welcomeStr2;
	
	welcomeStr = "Match the digital clock onto the";
	welcomeStr2 = "analogue clock . Press return...";
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, welcomeStr, *fontColor);	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);	
	
	SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, welcomeStr2, *fontColor);	
	SDL_BlitSurface(textSurface2, NULL, screen, &textLocation2);	
	
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
	SDL_FreeSurface(textSurface2);	
}

void render_correct_text() {
	//blank_out_background(0, 0, SCREEN_WIDTH, 50);
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "Press return...", foregroundColor, backgroundColor);
	//blank_out_background(0, 50, SCREEN_WIDTH, 50);
	// Pass zero for width and height to draw the whole surface 
	SDL_Rect textLocation = { (SCREEN_WIDTH/2)-50, 110, 0, 0 };
	
	//SDL_Color foregroundColor = { 255, 255, 255 }; 
	//SDL_Color backgroundColor = { 0, 255, 255 };		
		
	char* str;	
	
	str = "Correct!";
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, str, *backgroundColor);	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);	
		
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
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

void itoa(int n, char s[])
{
    int i, sign;
    if ((sign = n) < 0) /* record sign */
        n = -n; /* make n positive */
    i = 0;
    do { /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0); /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return;
}

void reverse ( char *s )
{
  int i, j;
  char c;
  for ( i = 0, j = strlen ( s ) - 1; i < j; i++, j-- ) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
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
