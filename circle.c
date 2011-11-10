/* Program Name: circle.c */
/* SDL screen test */
                                                                                
#include <SDL/SDL.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include <SDL/SDL_ttf.h>

/*http://tille.garrels.be/training/sdl/circle.php*/
                                                                                
/* Begin DrawPixel. */
void DrawPixel(SDL_Surface *screen, int x, int y,
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
/* End DrawPixel. */


void WriteClockText(char* txt, TTF_Font* font, SDL_Surface *screen, int x, int y)
{	
	SDL_Rect textLocation = { x, y, 0, 0 };
	
	SDL_Rect textLocation2 = { x+50, y+50, 0, 0 };
	
	SDL_Color foregroundColor = { 255, 255, 0 }; 
	SDL_Color backgroundColor = { 0, 0, 255 };
	
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "This is mytext.", foregroundColor, backgroundColor);
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, txt, foregroundColor, backgroundColor);
	//SDL_Surface* textSurface2 = TTF_RenderText_Shaded(font, "text", foregroundColor, backgroundColor);

	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 0, 0));
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	//SDL_BlitSurface(textSurface2, NULL, screen, &textLocation2);
	SDL_Flip(screen);
	
	SDL_FreeSurface(textSurface);
	//SDL_FreeSurface(textSurface2);
	
}

void add_hours(SDL_Surface *screen, int r, int x, int y)
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

}
                                                                         
void draw_circle(SDL_Surface *screen, int scr_wid, int scr_hi)
{	    
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
		DrawPixel(screen, xc, yc, 0, 255, 0 );
	  }
	/* end draw circle. */
	  
	 add_hours(screen, r, x, y);
}

