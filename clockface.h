                                                                               
#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>

void draw_pixel(SDL_Surface *screen, int x, int y,Uint8 R, Uint8 G,Uint8 B);
                                                                                
void draw_clock_face(SDL_Surface *screen, int scr_wid, int scr_hi);

void add_hours(SDL_Surface *screen, int r, int x, int y);

void draw_hand(SDL_Surface *screen);

void handle_mouse_down(int x, int y);

void handle_mouse_up(SDL_Surface *screen, int x, int y, int scr_wid, int scr_hi);

float get_angle_from_coords(int x, int y, int scr_wid, int scr_hi);