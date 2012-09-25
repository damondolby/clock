#include <SDL/SDL.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
# include <math.h>
#include "common.h"
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_GOES 2

int running;
int pre_game = 1;
SDL_Surface *screen;
TTF_Font* font;
TTF_Font* bigger_font;
char time_to_display[5];
int correct_times = 0;
int total_times = 0;
int mouse_x;
int mouse_y;
int goes_left;
time_t start,end;
int generated_hour;
int generated_min;


int init();
int run();
int events(SDL_Event *event);
int does_selection_match();
void make_background_flash();
int generate_new_time();
double get_time_taken();
void blank_out_background(int x, int y, int w, int h, SDL_Color* col);
void display_new_time();
void render_digital_clock();
void render_timer_bg();
void render_timer();
void render_score();
void render_welcome_screen();
void render_final_screen();
void render_correct_text();
int cleanup() ;
int get_hour();
int get_minute();
int round5(int m);
int hour_min_to_str(int h, int m, char* s);
void itoa(int n, char s[]);
void reverse ( char *s );