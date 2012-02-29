/* Program Name: circle.c */
/* SDL screen test */
                                                                                
#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>
#include "clockface.h"


/*http://tille.garrels.be/training/sdl/circle.php*/



//coords *hour_hand[RADIUS];
//coords *min_hand[RADIUS];

//hand *hour;
//hand *minute;



//int hour_hand_moving = 0; //TODO: change so part of new 'hand struct'
                                                                                
/* Begin draw_pixel. */
void draw_pixel(int x, int y, Uint32 color)
{
	
	//printf("x=%i, y=%i\n", x, y);
/* Don't fail on attempts to draw outside the screen. 
	
	http://www.math.com/tables/geometry/circles.htm
	*/
if ((x>=640) || (x<0)) return;
if ((y>=480) || (y<0)) return;

    //Uint32 color = SDL_MapRGB(clockface->screen->format, R, G, B);

    if ( SDL_MUSTLOCK(clockface->screen) ) {
        if ( SDL_LockSurface(clockface->screen) < 0 ) {
            return;
        }
    }
    
    switch (clockface->screen->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)clockface->screen->pixels + y*clockface->screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)clockface->screen->pixels + y*clockface->screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)clockface->screen->pixels + y*clockface->screen->pitch + x * 3;
            if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            } else {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;
                                                                                
        case 4: { /* Probably 32-bpp */
            Uint32 *bufp;
                                                                                
            bufp = (Uint32 *)clockface->screen->pixels + y*clockface->screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(clockface->screen) ) {
        SDL_UnlockSurface(clockface->screen);
    }
    SDL_UpdateRect(clockface->screen, x, y, 1, 1);
}
/* End draw_pixel. */


void write_clock_text(char* txt, TTF_Font* font, int x, int y)
{	
	SDL_Rect textLocation = { x, y, 0, 0 };
	
	SDL_Rect textLocation2 = { x+50, y+50, 0, 0 };
	
	SDL_Color foregroundColor = { 255, 255, 0 }; 
	//SDL_Color backgroundColor = { 0, 0, 255 };
	
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "This is mytext.", foregroundColor, backgroundColor);
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, txt, foregroundColor);
	//SDL_Surface* textSurface2 = TTF_RenderText_Shaded(font, "text", foregroundColor, backgroundColor);

	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 0, 0));
	SDL_BlitSurface(textSurface, NULL, clockface->screen, &textLocation);
	//SDL_BlitSurface(textSurface2, NULL, screen, &textLocation2);
	SDL_Flip(clockface->screen);
	
	SDL_FreeSurface(textSurface);
	//SDL_FreeSurface(textSurface2);
	
}

/*void add_hours(SDL_Surface *screen, int r, int x, int y)
{
	TTF_Font* font = TTF_OpenFont("FreeSans.ttf", 18);
	//int clock_middle_x = 115;
	//int clock_middle_y = 100;
	
	//int clock_inner_r = 80;
	
	//int offset = -15;
	
	//apply offsets so numbers fit inside the clock (possibly a more consistent/less hacky way to do this???)
	r = r - 20;
	x = x - 6;
	y = y - 15;
	
	//printf("%f\n", clock_middle_y - (clock_r-20)*sin(M_PI/2.0));
	//printf("%f\n", clock_middle_y - (clock_r-20)*sin((3/2.0)*M_PI));
	write_clock_text("12", font, screen, 
					x, 
					y - (r)*sin(M_PI/2.0));
	write_clock_text("3", font, screen, 
					x + (r)*cos(0*M_PI), 
					y);
	write_clock_text("6", font, screen, 
					x, 
					y - (r)*sin((3/2.0)*M_PI));
	write_clock_text("9", font, screen, 
					x + (r)*cos(M_PI), 
					y);
					
	TTF_CloseFont(font);

}*/

/* The Itoa code is in the public domain */
/*char* Itoa(int value, char* str, int radix) {
    static char dig[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    int n = 0, neg = 0;
    unsigned int v;
    char* p, *q;
    char c;
 
    if (radix == 10 && value < 0) {
        value = -value;
        neg = 1;
    }
    v = value;
    do {
        str[n++] = dig[v%radix];
        v /= radix;
    } while (v);
    if (neg)
        str[n++] = '-';
    str[n] = '\0';
 
    for (p = str, q = p + (n-1); p < q; ++p, --q)
        c = *p, *p = *q, *q = c;
    return str;
}*/

void add_numbers()
{
	int i, r;	
	char s[3];
	TTF_Font* font = TTF_OpenFont("FreeSans.ttf", 18);
	
	//apply adjustments so fonts look nice on the clock face
	int x = clockface->centre->x - 10;
	int y = clockface->centre->y - 10;
	r = RADIUS - 20;
	
	for (i=1; i<=12; i++)
	{
	
		int hand_minute;
		float pi_factor;
		hand_minute = (i*5) - 15;
		//s[0] =  i + '0';
		//Itoa(i, s, 1);
		
		//char buffer[10];
		snprintf(s, 3,"%d",i);
		
		pi_factor = M_PI * ((hand_minute * 6) / 180.0);
		//printf("Sum: %f\n", (hand_minute * 6) / 180.0);
		//printf("hand: %i, pi_factor: %f\n", hand_minute, pi_factor);
		
		int xc = x + r*cos(pi_factor);
		int yc = y + r*sin(pi_factor);
		//draw_pixel(screen, xc, yc, 0, 255, 0 );}
		
		write_clock_text(s, font, 
						xc, 
						yc);
	}
	TTF_CloseFont(font);
}

coords * point_new(int x, int y)
{
  coords *c;
  if((c = malloc(sizeof *c)) != NULL)
  {
    c->x = x;
    c->y = y;
  }
  return c;
}

//TODO: change x and y to rx and ry to indicate centre of the circle
//void draw_hand(SDL_Surface *screen, int r, int x, int y, int hand_minute, coords** cs, Uint8 R, Uint8 G, Uint8 B)
void add_hand(hand* h)
{
	draw_hand(h, h->color);
}

void remove_hand(hand* h)
{
	draw_hand(h,  SDL_MapRGB(clockface->screen->format, 0, 0, 0));
}

void draw_hand(hand* h, Uint32 color)
{
	int i;
	float pi_factor;
	int hand_minute;
	
	//r = r - 50;
	
	//TODO: this is very hacky. Need to find a better way to convert cartesian coordinates (x and y) to polar coordinates  (radius and angle) of cicle from clock minute
	hand_minute = h->minute - 15;
	
	pi_factor = M_PI * ((hand_minute * 6) / 180.0);
	//printf("Sum: %f\n", (hand_minute * 6) / 180.0);
	printf("hand: %i, pi_factor: %f\n", hand_minute, pi_factor);
	
	for (i=0; i<h->radius; i++)
	{		
		int xc = clockface->centre->x + i*cos(pi_factor);
		int yc = clockface->centre->y + i*sin(pi_factor);
		//printf("%dth element - xc: %d, xy: %d\n", i, xc, yc);
		//coords *coord  = {xc, yc};
		coords *coord  = point_new(xc, yc);
		//hour_hand[i] = &coord;                                                                                      
		h->xy[i] = coord;
		draw_pixel(xc, yc, color);
	}
}

void draw_face()
{
	//int r = 150;
	//int x = scr_wid/2;
	//int y = scr_hi - RADIUS - 20;
	
	/* draw a circle.  The constant M_PI is defined from math.h. */
	float i;
											
	for (i=0; i<2*M_PI; i+=0.01) {
		int xc = clockface->centre->x + RADIUS*cos(i);
		int yc = clockface->centre->y + RADIUS*sin(i);
		draw_pixel(xc, yc, clockface->color);
	  }
}
                                                                         
void init_clock_face(SDL_Surface *screen, int scr_wid, int scr_hi)
{	    
	printf ("bytesperpixel: %i\n", screen->format->BytesPerPixel);
	//printf("circle test");
	//return;
	
	//From radius and screen width/height set centre of the circle rx and ry
	//TODO: pass these into funciton once refactored? (These are defined in two functions)
	//TODO: change to rx and ry (i.e. to indicate centre of the circle)
	//int r = 150;
	//int origin_x = scr_wid/2;
	//int origin_y = scr_hi - RADIUS - 20;	
	
	//printf("1. draw clock face\n");
	if((clockface = malloc(sizeof *clockface)) != NULL)
	  {	  
		 if((clockface->centre = malloc(sizeof *clockface->centre)) != NULL)
		{
			//From radius and screen width/height set centre of the circle rx and ry
			clockface->centre->x = scr_wid/2;
			clockface->centre->y = scr_hi - RADIUS - 20;		
			clockface->screen = screen;
			clockface->color = SDL_MapRGB(clockface->screen->format, 255, 0, 0);
		}		
	  }
	
	//printf("2. draw clock face\n");	  
	  //TODO: can clockface be passed around here now?
	draw_face();
	  
	/* end draw circle. */
	  
	 //add_hours(screen, r, x, y);	  
	  
	  printf("2. draw clock face - add hour\n");
	  //if((hour = malloc(sizeof *hour)) != NULL)
	  if((clockface->hour = malloc(sizeof *clockface->hour)) != NULL)
	  {	  
		  clockface->hour->minute = 50;
		  clockface->hour->radius = RADIUS-50;
		  clockface->hour->color = SDL_MapRGB(clockface->screen->format, 0, 0, 255);
		  add_hand(clockface->hour);
	  }
	  
	  printf("3. draw clock face - add minute\n");
	   if((clockface->minute = malloc(sizeof *clockface->minute)) != NULL)
	  {	  
		  clockface->minute->minute = 15;
		  clockface->minute->radius = RADIUS-75;
		  clockface->minute->color = SDL_MapRGB(clockface->screen->format, 0, 255, 0);
		  add_hand(clockface->minute);
	  }
	  
	  //int i2 = 29;
	  //printf("%dth element - xc: %d, xy: %d\n", i2, hour_hand[i2]->x, hour_hand[i2]->y);
	  //i2 = 89;
	  //printf("%dth element - xc: %d, xy: %d\n", i2, hour_hand[i2]->x, hour_hand[i2]->y);
	  
	  add_numbers();
}

/*If the co-ordinates are 'close' to either clock hands then return to true
as 'colliding'
*/
int hand_collision(int x, int y, hand* h)
{
	int i, r;	
	//coords *c;
	
	r = 0;
	
	printf("1. hand_collision\n");
	//printf("hand collision - mouse coords: %d, y: %d\n", x, y);
	//printf("5. %dth element: x=%d, y=%d\n", 10, hour_hand[10]->x, hour_hand[10]->y);
	
	//TODO: change so it checks collision for both hands
	//for (i=0, c = hour->xy[i]; i<RADIUS && c != NULL; i++)
	for (i=0; i<h->radius && h->xy[i] != NULL; i++)
	{
		//printf("2. hand_collision\n");
		//TODO: Need to modify this collision so more precise		
		//printf("hand collision - hand coords, i: %d, x: %d, y: %d\n", i, hour->xy[i]->x, hour->xy[i]->y);
		//if (c->x == x || c->y == y)
		//TODO: can this logic be centralised somehow in hand struct?
		if ((h->xy[i]->x >= x-3 && h->xy[i]->x <= x+3) &&
			(h->xy[i]->y >= y-3 && h->xy[i]->y <= y+3))
		{
			//printf("3. hand_collision\n");
			//hour_hand_moving = 1;
			h->is_moving = 1;
			r = 1;
			break;
		}
	} 
	printf("4. hand_collision\n");
	return r;
}

void handle_mouse_down(int x, int y)
{
	int is_collision;
	printf("3. mouse down\n");
	is_collision = hand_collision(x, y, clockface->hour);
	printf("4. mouse down\n");
	if (is_collision == 0)
		is_collision = hand_collision(x, y, clockface->minute);
	printf("5. mouse down\n");
}

//Returns polar coordinate angle (-iPI to +PI) from cartesian coordinates (i.e. x, y)
float get_angle_from_coords(int x, int y)
{
	float angle;
	
	//From radius and screen width/height set centre of the circle rx and ry
	//TODO: pass these into funciton once refactored? (These are defined in two functions)
	//int r = 150;
	//int rx = scr_wid/2;
	//int ry = scr_hi - RADIUS - 20;
	
	printf("Getting Angle. x=%d, y=%d, rx=%d, ry=%d\n", clockface->centre->x, clockface->centre->y, x, y);
			
	//angle = (atan2(ry-y, x-rx) / M_PI) * 180;	
	angle = atan2(clockface->centre->y-y, x-clockface->centre->x);
	//angle = atan2(clockface->centre->y-y, clockface->centre->x-x);
	
	return angle;
}

/*
Takes polar coordinate angle (-PI to +PI) and returns clock minute.

Possibly there is a simpler (less complicated way to achieve this).
*/
int convert_angle_to_clock_minute(float angle)
{
	int minute;
	
	if (angle < 0)
	{
		//This is where  the cartesian y coordinate was less than zero (i.e. between minutes 15 and 45 on the clock)
		angle = angle * -1;
		angle = angle + M_PI;
	}
	
	//printf("2. mouse up after hand move attempt. Angle: %f, \n", angle);
	
	//Now angle is 0 through to a max of 2 PI in a anti-clockwise direction (starting at 90 degress on a circle - i.e. 15 m inutes)
	
	//Convert to minute	(i.e. if PI then should get 30 - which is 45 minute position before offset below)
	minute = (angle / M_PI) * 30;
	
	//printf("3. mouse up after hand move attempt. Angle: %f, Minute: %d\n", angle, minute);
	
	// Apply 15 minute offset because polar coordinate 0 starts at the 15 minute position
	minute = minute - 15;
	
	//printf("4. mouse up after hand move attempt. Angle: %f, Minute: %d\n", angle, minute);
	
	// Anything between the 0 to 15 mminute range will have ended up negative (so multiply by -1 to turn positive). 
	//For anything < 15 subtract from 60 (remmbering polar coordinates work in an anti-clockwise direction - this is for anything between 45 and 60 minutes on the clock)
	//Seems to work but don't fully understand Maths behind it.
	if (minute < 0)
		minute = minute * -1;
	else if (minute < 15)
	{		
		minute = 60 - minute;
	}
	
	return minute;
		
}

void move_hand(int x, int y, hand *h)
{
	float angle = get_angle_from_coords(x, y);
	//printf("1. mouse up after hand move attempt. Angle: %f, \n", angle);
	int minute = convert_angle_to_clock_minute(angle);
	//printf("20. mouse up after hand move attempt. Angle: %f, Minute: %d\n", angle, minute);		
	
	//draw_hand(screen, r, x, y, 30, hour_hand);
	
	//draw_face(screen, scr_wid, scr_hi);
	
	//printf("1. move_hand. angle: %f, minute: %d\n", angle, minute);
	
	//int r = 150;
	//int hx = scr_wid/2;
	//int hy = scr_hi - RADIUS - 20;
	
	//printf("1. %dth element: x=%d, y=%d\n", 10, hour_hand[10]->x, hour_hand[10]->y);
	//draw_hand(h, 0, 0, 0);
	//TODO: better way to remove hands?
	remove_hand(h);
	
	h->is_moving = 0;
	h->minute = minute;
	
	//printf("2. %dth element: x=%d, y=%d\n", 10, hour_hand[10]->x, hour_hand[10]->y);
	add_hand(h);
	//printf("3. %dth element: x=%d, y=%d\n", 10, hour_hand[10]->x, hour_hand[10]->y);
}

void handle_mouse_up(int x, int y)
{	
	if (clockface->hour->is_moving == 1)
	{
		//printf("1. handle_mouse_up - hour. x=%d, y=%d\n", x, y);
		//Move hour hand
		move_hand(x, y, clockface->hour);
	}
	else if (clockface->minute->is_moving == 1)
	{
		//printf("2. handle_mouse_up - hour\n");
		//Move minute hand
		move_hand(x, y, clockface->minute);
	}
}




