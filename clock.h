#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
# include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int Running;
SDL_Surface *screen;
TTF_Font* font;
int NewTime;
char TimeToDisplay[5];
int mouse_x;
int mouse_y;


int init();
int events(SDL_Event *event);
int run();
int loop();
void draw_rectangle();
int render();
int cleanup() ;
int getHour();
int getMinute();
int round5(int m);
int hourMinToStr(int h, int m, char* s);