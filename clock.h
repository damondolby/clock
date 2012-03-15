#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
# include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int Running;
SDL_Surface *screen;
TTF_Font* font;
//int NewTime;
char timeToDisplay[5];
int correctTimes = 0;
int totalTimes = 0;
int mouse_x;
int mouse_y;

int generated_hour;
int generated_min;


int init();
int events(SDL_Event *event);
int run();
int generate_new_time();
void blank_out_background(int x, int y, int w, int h);
void render_digital_clock();
void render_score();
int cleanup() ;
int getHour();
int getMinute();
int round5(int m);
int hourMinToStr(int h, int m, char* s);
int does_selection_match();
void itoa(int n, char s[]);
void reverse ( char *s );