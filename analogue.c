#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>
#include "analogue.h"

/*Draw one pixel - useful for plotting clock face circle outline and hands*/
void draw_pixel(int x, int y, Uint32 color) {
	
	// Don't fail on attempts to draw outside the screen
	if ((x>=640) || (x<0)) return;
	if ((y>=480) || (y<0)) return;

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
		    } 
		    else {
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

/*Write text using specified font at specified location (x,y)*/
void write_clock_text(char* txt, TTF_Font* font, int x, int y) {	
	
	SDL_Rect textLocation = { x, y, 0, 0 };	
	
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, txt, *font_color);	

	SDL_BlitSurface(textSurface, NULL, clockface->screen, &textLocation);
	SDL_Flip(clockface->screen);	
	SDL_FreeSurface(textSurface);
}

/*Add the 12 hours to the clock face*/
void add_numbers() {
	int i, r;	
	char s[3];
	TTF_Font* font = TTF_OpenFont("FreeSans.ttf", 18);
	
	//apply adjustments so fonts look nice on the clock face
	int x = clockface->centre->x - 10;
	int y = clockface->centre->y - 10;
	r = RADIUS - 20;
	
	for (i=1; i<=12; i++) {
	
		int hand_minute;
		float pi_factor;
		hand_minute = (i*5) - 15;

		snprintf(s, 3,"%d",i);
		
		pi_factor = M_PI * ((hand_minute * 6) / 180.0);
		
		int xc = x + r*cos(pi_factor);
		int yc = y + r*sin(pi_factor);
		
		write_clock_text(s, font, xc, yc);
	}
	TTF_CloseFont(font);
}

/*Create a new x,y point struct*/
coords * point_new(int x, int y) {
	coords *c;
	if((c = malloc(sizeof *c)) != NULL) {
		c->x = x;
		c->y = y;
	}
	return c;
}

/*Remove hands by drawing over the current hand with the clock background colour, so it 'disappears'*/
void remove_hand(hand* h) {
	draw_hand(h,  SDL_MapRGB(clockface->screen->format, background_color->r, background_color->g, background_color->b));
}

/*Draw (or redraw) the hour and minute hands*/
void draw_hands() {
	draw_hand(clockface->hour, clockface->hour->color);
	draw_hand(clockface->minute, clockface->minute->color);
}

/*Draw (or redraw) the hand h with specified colour*/
void draw_hand(hand* h, Uint32 color) {
	int i;
	float pi_factor;
	int hand_minute;
	
	//TODO: this is very hacky. Need to find a better way to convert cartesian coordinates (x and y) to polar coordinates  (radius and angle) of cicle from clock minute
	hand_minute = h->minute - 15;
	
	pi_factor = M_PI * ((hand_minute * 6) / 180.0);
	
	for (i=0; i<h->radius; i++) {		
		int xc = clockface->centre->x + i*cos(pi_factor);
		int yc = clockface->centre->y + i*sin(pi_factor);
		coords *coord  = point_new(xc, yc);
		h->xy[i] = coord;
		draw_pixel(xc, yc, color);
	}
}

/*Draws the clock face circle outline*/
void draw_face() {
	
	/* draw a circle.  The constant M_PI is defined from math.h. */
	float i;
											
	for (i=0; i<2*M_PI; i+=0.01) {
		int xc = clockface->centre->x + RADIUS*cos(i);
		int yc = clockface->centre->y + RADIUS*sin(i);
		draw_pixel(xc, yc, clockface->color);
	  }
}
 
/*Initialise function that draws the clock face with hands and numbers.*/
void init_clock_face(SDL_Surface *screen, int scr_wid, int scr_hi) {	    
	if((clockface = malloc(sizeof *clockface)) != NULL) {	  
		 if((clockface->centre = malloc(sizeof *clockface->centre)) != NULL) {
			//From radius and screen width/height set centre of the circle rx and ry
			//Circle is two thirds of the way across the screen.
			clockface->centre->x = (scr_wid/3)*2;
			clockface->centre->y = scr_hi - RADIUS - 20;		
			clockface->screen = screen;
			clockface->color = SDL_MapRGB(clockface->screen->format, font_color->r, font_color->g, font_color->b);
		}		
	  }
	
	//TODO: can clockface be passed around here now?
	draw_face();
	  
	if((clockface->hour = malloc(sizeof *clockface->hour)) != NULL) {	  
		clockface->hour->minute = 15;
		clockface->hour->radius = RADIUS-75;
		clockface->hour->color = SDL_MapRGB(clockface->screen->format, font_color->r, font_color->g, font_color->b);
	  }
	  
	if((clockface->minute = malloc(sizeof *clockface->minute)) != NULL) {	  
		clockface->minute->minute = 40;
		clockface->minute->radius = RADIUS-50;
		clockface->minute->color = SDL_MapRGB(clockface->screen->format, font_color->r, font_color->g, font_color->b);
	  }
	  
	  draw_hands();
	  	  
	  add_numbers();
}

/*If the co-ordinates are 'close' to either clock hands then return to true as 'colliding'*/
int hand_collision(int x, int y, hand* h) {
	
	int i, r;		
	r = 0;	
	
	for (i=0; i<h->radius && h->xy[i] != NULL; i++) {
		//TODO: can this logic be centralised somehow in hand struct?
		if ((h->xy[i]->x >= x-3 && h->xy[i]->x <= x+3) &&
			(h->xy[i]->y >= y-3 && h->xy[i]->y <= y+3)) {
			h->is_moving = 1;
			r = 1;
			break;
		}
	} 
	return r;
}

/*Works out if any hand is closer enough to the mouse cursor (x,y) (usually called when the mouse button is pressed)*/
void handle_mouse_down(int x, int y) {
	
	int is_collision;
	is_collision = hand_collision(x, y, clockface->hour);
	if (is_collision == 0)
		is_collision = hand_collision(x, y, clockface->minute);
	
	if (is_collision == 1)
		event_count = 0;
}

/*Returns polar coordinate angle (-iPI to +PI) from cartesian coordinates (i.e. x, y)*/
float get_angle_from_coords(int x, int y) {
	
	float angle;
	angle = atan2(clockface->centre->y-y, x-clockface->centre->x);	
	return angle;
}

/*
Returns the minute of the clockface for any angle of the clock.
Takes polar coordinate angle (-PI to +PI) and returns clock minute.
Possibly there is a simpler (less complicated way to achieve this).
*/
int convert_angle_to_clock_minute(float angle) {
	int minute;
	
	if (angle < 0) {
		//This is where  the cartesian y coordinate was less than zero (i.e. between minutes 15 and 45 on the clock)
		angle = angle * -1;
		angle = angle + M_PI;
	}	
	
	//Now angle is 0 through to a max of 2 PI in a anti-clockwise direction (starting at 90 degress on a circle - i.e. 15 m inutes)	
	//Convert to minute	(i.e. if PI then should get 30 - which is 45 minute position before offset below)
	minute = (angle / M_PI) * 30;	
	
	// Apply 15 minute offset because polar coordinate 0 starts at the 15 minute position
	minute = minute - 15;	
	
	// Anything between the 0 to 15 mminute range will have ended up negative (so multiply by -1 to turn positive). 
	//For anything < 15 subtract from 60 (remmbering polar coordinates work in an anti-clockwise direction - this is for anything between 45 and 60 minutes on the clock)
	//Seems to work but don't fully understand Maths behind it.
	if (minute < 0)
		minute = minute * -1;
	else if (minute < 15) {		
		minute = 60 - minute;
	}
	
	return minute;		
}

/*Move hand to new location based on x,y coord that can be at any point on the circle radius.
Resulting minute of clock gets rounded to nearest 5 (i.e. minute hand can only get moved to every 5 minute segment
and hour hand can only get moved to every 1 hour segment of the clockface).
*/
void move_hand(int x, int y, hand *h) {
	
	float angle = get_angle_from_coords(x, y);
	int minute = round5(convert_angle_to_clock_minute(angle));

	//TODO: better way to remove hands?
	remove_hand(h);
		
	h->minute = minute;
}

/*Hand that was just being moved gets redrawn to its 'final' destination */
void handle_mouse_up(int x, int y) {	
	
	if (clockface->hour->is_moving == 1) {
		//Move hour hand
		move_hand(x, y, clockface->hour);
		clockface->hour->is_moving = 0;
	}
	else if (clockface->minute->is_moving == 1) {
		//Move minute hand
		move_hand(x, y, clockface->minute);
		clockface->minute->is_moving = 0;
	}
	
	draw_hands();
}

/*Redraw the hand that is currently moving but only every EVENT_SKIP events (uses too much CPU to redraw after every event).*/
void handle_mouse_move(int x, int y) {	
	event_count++;
	
	//Only move for every X events (as too slow to try and move for every event)
	if (event_count % EVENT_SKIP == 0) {
		if (clockface != NULL) {
			if (clockface->hour->is_moving == 1) {
				//Move hour hand
				move_hand(x, y, clockface->hour);
			}
			else if (clockface->minute->is_moving == 1) {
				//Move minute hand
				move_hand(x, y, clockface->minute);
			}
			
			draw_hands();
		}
	}
}


int get_selected_hour() {
	
	//Slight hack as 0 makes no sense when comparing hours to a digital clock (will always expect to be 12). Possibly a better way to do this?
	if (clockface->hour->minute == 0)
		return 12;
	else
		return clockface->hour->minute/5;
}

int get_selected_min() {
	
	//Slight hack as 60 makes no sense when comparing minutes to a digital clock (will always expect to be 0). Possibly a better way to do this?
	if (clockface->minute->minute == 60)
		return 0;
	else
		return clockface->minute->minute;
}



