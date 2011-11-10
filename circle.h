                                                                               
#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>

void DrawPixel(SDL_Surface *screen, int x, int y,Uint8 R, Uint8 G,Uint8 B);
                                                                                
void draw_circle(SDL_Surface *screen, int scr_wid, int scr_hi);

void add_hours(SDL_Surface *screen, int r, int x, int y);