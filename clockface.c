/* Program Name: circle.c */
/* SDL screen test */
                                                                                
#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>

/*http://tille.garrels.be/training/sdl/circle.php*/
                                                                                
/* Begin draw_pixel. */
void draw_pixel(SDL_Surface *screen, int x, int y,
Uint8 R, Uint8 G,
Uint8 B)
{
	
	//printf("x=%i, y=%i\n", x, y);
/* Don't fail on attempts to draw outside the screen. 
	
	http://www.math.com/tables/geometry/circles.htm
	*/
if ((x>=640) || (x<0)) return;
if ((y>=480) || (y<0)) return;

    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
    
    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
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
                                                                                
            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    SDL_UpdateRect(screen, x, y, 1, 1);
}
/* End draw_pixel. */


void WriteClockText(char* txt, TTF_Font* font, SDL_Surface *screen, int x, int y)
{	
	SDL_Rect textLocation = { x, y, 0, 0 };
	
	SDL_Rect textLocation2 = { x+50, y+50, 0, 0 };
	
	SDL_Color foregroundColor = { 255, 255, 0 }; 
	//SDL_Color backgroundColor = { 0, 0, 255 };
	
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "This is mytext.", foregroundColor, backgroundColor);
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, txt, foregroundColor);
	//SDL_Surface* textSurface2 = TTF_RenderText_Shaded(font, "text", foregroundColor, backgroundColor);

	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 0, 0));
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	//SDL_BlitSurface(textSurface2, NULL, screen, &textLocation2);
	SDL_Flip(screen);
	
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
	WriteClockText("12", font, screen, 
					x, 
					y - (r)*sin(M_PI/2.0));
	WriteClockText("3", font, screen, 
					x + (r)*cos(0*M_PI), 
					y);
	WriteClockText("6", font, screen, 
					x, 
					y - (r)*sin((3/2.0)*M_PI));
	WriteClockText("9", font, screen, 
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

void add_numbers(SDL_Surface *screen, int r, int x, int y)
{
	int i;	
	char s[3];
	TTF_Font* font = TTF_OpenFont("FreeSans.ttf", 18);
	
	//apply adjustments so fonts look nice on the clock face
	x = x - 10;
	y = y - 10;
	r = r - 20;
	
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
		
		WriteClockText(s, font, screen, 
						xc, 
						yc);
	}
	TTF_CloseFont(font);
}

void draw_hand(SDL_Surface *screen, int r, int x, int y, int hand_minute)
{
	int i;
	float pi_factor;
	
	r = r - 50;
	
	hand_minute = hand_minute - 15;
	
	pi_factor = M_PI * ((hand_minute * 6) / 180.0);
	printf("Sum: %f\n", (hand_minute * 6) / 180.0);
	printf("hand: %i, pi_factor: %f\n", hand_minute, pi_factor);
	
	for (i=0; i<r; i++)
	{
		int xc = x + i*cos(pi_factor);
		int yc = y + i*sin(pi_factor);
		draw_pixel(screen, xc, yc, 0, 255, 0 );
	}
}
                                                                         
void draw_clock_face(SDL_Surface *screen, int scr_wid, int scr_hi)
{	    
	printf ("bytesperpixel: %i\n", screen->format->BytesPerPixel);
	//printf("circle test");
	//return;
	
	int r = 150;
	int x = scr_wid/2;
	int y = scr_hi - r - 20;
	
	
    //TTF_Font* font = TTF_OpenFont("FreeSans.ttf", 26);	
	
	//x + (r-20)*cos(90),
	//float test = sin(M_PI/2.0);
	//float test2 = 3/2.0;
	//printf("%f\n", test);
	//printf("%f\n", test2);
	//printf("%f\n", sin((3/2.0)*M_PI));
	
  
	/* draw a circle.  The constant M_PI is defined from math.h. */
	float i;
											
	for (i=0; i<2*M_PI; i+=0.01) {
		int xc = x + r*cos(i);
		int yc = y + r*sin(i);
		draw_pixel(screen, xc, yc, 0, 255, 0 );
	  }
	/* end draw circle. */
	  
	 //add_hours(screen, r, x, y);
	  
	  //draw_hand(screen, r, x, y, 15);
	  
	  //draw_hand(screen, r, x, y, 0);
	  //draw_hand(screen, r, x, y, 15);
	  //draw_hand(screen, r, x, y, 30);
	  //draw_hand(screen, r, x, y, 45);
	  draw_hand(screen, r, x, y, 60);
	  //draw_hand(screen, r, x, y, 5);
	  //draw_hand(screen, r, x, y, 10);
	  //draw_hand(screen, r, x, y, 15);
	  //draw_hand(screen, r, x, y, 20);
	  //draw_hand(screen, r, x, y, 25);
	  //draw_hand(screen, r, x, y, 30);
	  //draw_hand(screen, r, x, y, 35);
	  //draw_hand(screen, r, x, y, 40);
	  draw_hand(screen, r, x, y, 45);
	  //draw_hand(screen, r, x, y, 50);
	  //draw_hand(screen, r, x, y, 55);
	  add_numbers(screen, r, x, y);
}

