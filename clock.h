#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
# include <math.h>
#include "common.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//64-224-208

int Running;
int pre_game = 1;
SDL_Surface *screen;
TTF_Font* font;
TTF_Font* biggerFont;
//int NewTime;
char timeToDisplay[5];
int correctTimes = 0;
int totalTimes = 0;
int mouse_x;
int mouse_y;
//SDL_Color foregroundColor = { 255, 255, 255 };	
//SDL_Color* backgroundColor2;		
//Uint32 bg_top = SDL_MapRGB(screen->format, 0x1e, 0x90, 0xff);
//Uint32 bg_flash = SDL_MapRGB(screen->format, 0x00, 0x00, 0x80);
//Uint32 bg_bottom = SDL_MapRGB(screen->format, 0xb0, 0xc4, 0xde);

int generated_hour;
int generated_min;


int init();
int events(SDL_Event *event);
int run();
int generate_new_time();
void blank_out_background(int x, int y, int w, int h, SDL_Color* col);
void display_new_time();
void render_digital_clock();
void render_score();
void render_welcome_screen();
void render_correct_text();
int cleanup() ;
int getHour();
int getMinute();
int round5(int m);
int hourMinToStr(int h, int m, char* s);
int does_selection_match();
void make_background_flash();
void itoa(int n, char s[]);
void reverse ( char *s );