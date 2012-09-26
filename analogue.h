                                                                               
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

void draw_pixel(int x, int y, Uint32 color);
void write_clock_text(char* txt, TTF_Font* font, int x, int y);
void add_numbers();
coords * point_new(int x, int y);
void remove_hand(hand* h);
void draw_hands();
void draw_hand(hand* h, Uint32 color);
void draw_face();                                                                                
void init_clock_face(SDL_Surface *screen, int scr_wid, int scr_hi);
int hand_collision(int x, int y, hand* h);
void handle_mouse_down(int x, int y);
float get_angle_from_coords(int x, int y);
int convert_angle_to_clock_minute(float angle);
void move_hand(int x, int y, hand *h);
void handle_mouse_up(int x, int y);
void handle_mouse_move(int x, int y);
int get_selected_hour();
int get_selected_min();

