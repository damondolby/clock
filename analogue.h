                                                                               
#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>
#include "common.h"

#define RADIUS 150
#define EVENT_SKIP 5 /*Number of events to skip when dragging so not continutally redrawing (which takes time)*/

typedef struct {
	int x;
	int y;
} coords;

typedef struct {
	coords *xy[RADIUS];
	int minute;
	int is_moving;	
	int radius;
	Uint32 color;
} hand;

typedef struct {
	hand *hour;
	hand *minute;
	coords *centre;
	SDL_Surface *screen;
	Uint32 color;
} clock;

clock *clockface;

int event_count; /*Event count when dragging a hand*/

/*Public functions called from clock.c*/
void init_clock_face(SDL_Surface *screen, int scr_wid, int scr_hi);
void handle_mouse_down(int x, int y);
void handle_mouse_up(int x, int y);
void handle_mouse_move(int x, int y);

/*Internal functions to analogue.c*/
void draw_face();
void draw_pixel(int x, int y, Uint32 color);
void add_numbers();
void write_clock_text(char* txt, TTF_Font* font, int x, int y);
void draw_hands();
void draw_hand(hand* h, Uint32 color);                                 
coords * point_new(int x, int y);
void move_hand(int x, int y, hand *h);
void remove_hand(hand* h);
int hand_collision(int x, int y, hand* h);
float get_angle_from_coords(int x, int y);
int convert_angle_to_clock_minute(float angle);
int get_selected_hour();
int get_selected_min();

