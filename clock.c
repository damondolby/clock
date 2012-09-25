#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include "clock.h"
#include <math.h>

int main() {
	
	//gcc clock.c analogue.c -lSDL -lSDL_ttf -o clock.out
	
	init();
	
	run();
	
	cleanup();	
	
	return 0;
}

int init() {
	
	//Configure screen	
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
	
	//initialise fonts and colours
	font = TTF_OpenFont("FreeSans.ttf", 26);
	bigger_font = TTF_OpenFont("FreeSans.ttf", 56);	
	
	backgroundColor = malloc(sizeof *backgroundColor);
	backgroundColor->r = 154;
	backgroundColor->g = 192;
	backgroundColor->b= 205;
	
	fontColor = malloc(sizeof *fontColor);
	fontColor->r = 16;
	fontColor->g = 78;
	fontColor->b= 139;
	
	running = 1;
	
	blank_out_background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	render_welcome_screen();
}

int run() {
	
	SDL_Event event;
	
	while (running) {
		
		while (SDL_PollEvent(&event))
		{			
			events(&event);	
		}
		
		//Delay to prevent high CPU
		SDL_Delay(100);
		
		//render timer each loop if it's in the middle of a game
		if (!pre_game)
			render_timer();
	}
}

int events(SDL_Event *event) {
	
	if (!pre_game)
		handle_mouse_move(event->motion.x, event->motion.y);
	
	switch( event->type ){
		case SDL_QUIT:
			running = 0;
			break;
		case SDL_KEYDOWN:			
			switch (event->key.keysym.sym) 
			{					
			   case SDLK_RETURN:
				   if (!pre_game)
					total_times++;
				   else
				   {
					//Reset goes left and time if it's pre-game
					goes_left = MAX_GOES;
					time (&start);
				   }
				   display_new_time();
				break;					
			   case SDLK_q:
				running = 0;
				break;
		      }
		      break;
		case SDL_KEYUP:				      
			break;
		
		case SDL_MOUSEBUTTONDOWN: 	
			switch(event->button.button) {
				case SDL_BUTTON_LEFT: 
					
					if (!pre_game)
					{
						//handle mouse events during a game
						mouse_x = event->motion.x;
						mouse_y = event->motion.y;
						handle_mouse_down(mouse_x, mouse_y);
					}
					break;
				
				case SDL_BUTTON_RIGHT: 
					break;
				
				case SDL_BUTTON_MIDDLE: 
				    break;						
			}
			break;
			
		case SDL_MOUSEBUTTONUP: 
			switch(event->button.button) {
				case SDL_BUTTON_LEFT:
					
					if (!pre_game)
					{
						//handle mouse events during a game
						mouse_x = event->motion.x;
						mouse_y = event->motion.y;					
						handle_mouse_up(mouse_x, mouse_y);					
						
						//check if analogue clock matches the digital clock
						does_selection_match();					
						get_time_taken() ;						
					}													
					break;			
			}
			break;
	}
}

int does_selection_match() {
	int retVal = 0;
	
	if (generated_hour == get_selected_hour() &&
		generated_min == get_selected_min())
	{
		//In here if analogue clock matches the digital clock
		goes_left--;
		
		if (goes_left == 0)
		{
			//In here if all goes are complete
			pre_game = 1;
			render_final_screen();
		}
		else
		{		
			//In here if player still has goes left
			make_background_flash();
			generate_new_time();
		}
	}		
}

/*Function makes it obvious that a user has matched the two clocks (by 'flashing')*/
void make_background_flash(){	
	render_correct_text();
	SDL_Delay(500);
	blank_out_background((SCREEN_WIDTH/2)-75, 100, 150, 50, backgroundColor);
}

/*Generate a new digital clock (and render score/timer)*/
int generate_new_time() {		
	
	int len;			
	
	//todo: time_t is a restricted type, and 
	//may not be meaningfully converted to unsigned int 
	//on some systems?
	srand ( (unsigned int)time ( NULL ) ); 
	generated_hour = get_hour();
	generated_min = get_minute();
	len = hour_min_to_str(generated_hour, generated_min, time_to_display);
	render_digital_clock();
	render_score();
	render_timer();	
}

/*Returns the length of time since the current game started*/
double get_time_taken() {
	double elapsed;
	time (&end);	
	
	elapsed = difftime (end,start);
	return elapsed;
}

/*'Blank out' chunk of screen so it can be redrawn*/
void blank_out_background(int x, int y, int w, int h, SDL_Color *col) {
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
}

/*Generate/Render/Display new digital clock*/
void display_new_time() {
	pre_game = 0;
	blank_out_background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	render_score();	
	generate_new_time();
	init_clock_face(screen, SCREEN_WIDTH, SCREEN_HEIGHT);		
}

void render_digital_clock() {	
	
	//Box behind digital clock (to create border effect)
	blank_out_background(20, SCREEN_HEIGHT/2, 200, 140, fontColor);
	
	blank_out_background(25, (SCREEN_HEIGHT/2)+5, 190, 130, backgroundColor);	
	
	SDL_Rect textLocation = { 50, (SCREEN_HEIGHT/2)+20, 0, 0 };
		
	SDL_Surface* textSurface = TTF_RenderText_Blended(bigger_font, time_to_display, *fontColor);
	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);			
}

void render_timer_bg() {
	
	blank_out_background(20, 20, 250, 140, fontColor);
	
	//Box behind text box (to create border effect)
	blank_out_background(25, 25, 240, 130, backgroundColor);
	
	SDL_Flip(screen);
}

void render_timer() {
	
	blank_out_background(50, 40, 210, 40, backgroundColor);
	
	SDL_Rect textLocation = { 50, 40, 0, 0 };		
	
	double time = get_time_taken();
	char str[50];

	itoa(time, str); //This will need enhancing to convert double if decimal places need to be displayed.
	strcat (str," seconds");
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, str, *fontColor);
	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);	
	
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
}

void render_score() {
	
	blank_out_background(50, 80, 210, 40, backgroundColor);
	
	SDL_Rect textLocation = { 50, 80, 0, 0 };

	char str[50], correctStr[3], totalStr[3];	
	
	itoa(correct_times, correctStr);
	itoa(total_times, totalStr);
	
	itoa(goes_left, str); 
	
	strcat (str," goes left");
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, str, *fontColor);
	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
}

void render_welcome_screen() {
	SDL_Rect textLocation = { 100, 200, 0, 0 };
	SDL_Rect textLocation2 = { 100, 230, 0, 0 };
		
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

void render_final_screen() {
	
	blank_out_background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	
	SDL_Rect textLocation = { 100, 200, 0, 0 };
	SDL_Rect textLocation2 = { 100, 230, 0, 0 };
	
	char* welcomeStr2;	
	
	char time_str[5];
	double time = get_time_taken();
	itoa(time, time_str);
	
	char str[50];
	strcpy (str,"All Done. You took ");
	strcat (str, time_str);
	strcat (str," seconds.");
	
	welcomeStr2 = "Press return to go again...";
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, str, *fontColor);	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);	
	
	SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, welcomeStr2, *fontColor);	
	SDL_BlitSurface(textSurface2, NULL, screen, &textLocation2);	
	
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
	SDL_FreeSurface(textSurface2);	
}

void render_correct_text() {	
	blank_out_background(25, (SCREEN_HEIGHT/2)+5, 190, 130, fontColor);
	
	SDL_Rect textLocation = { 40, (SCREEN_HEIGHT/2)+20, 0, 0 };	
		
	char* str;	
	
	str = "Good!";
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(bigger_font, str, *backgroundColor);	
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);	
		
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);	
}

int cleanup() {
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

int get_hour() {
	int x;
	x = rand(); // everytime it is different because the seed is different.
	return (x % 12) + 1;
}

int get_minute() {
	int x, m, r;
	char s;	
	x = rand(); // everytime it is different because the seed is different.
	m = x % 59;	
	
	return round5(m);
}

int round5(int m) {
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

int hour_min_to_str(int h, int m, char* s) {
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
	
	if (i == 4)
		s[4] = 0;
	return i;
}

void itoa(int n, char s[]) {
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

void reverse ( char *s ) {
  int i, j;
  char c;
  for ( i = 0, j = strlen ( s ) - 1; i < j; i++, j-- ) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}
