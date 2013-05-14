#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "sprout.h"

int main(int argc,char *argv[])
{
    int width = 800;
    int height = 602;
    std::string background = "images/background.jpg";

	bool gameRunning = true;
    bool ended = false;
	SDL_Event event;	// dump event polls into this

    // Initialize everything
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
	}

    // Cleanup on exit
    atexit(SDL_Quit);

    // get current display information (for height, width, color depth, etc.)
	//const SDL_VideoInfo* info = SDL_GetVideoInfo();
    //int width  = info->current_w*2/3;
    //int height = info->current_h*2/3;
    int depth  = 0; // Set to current screen depth

    // What we draw on in memory since the algorithms are pixel-based
    // We'll use 16 bpp since 8 doesn't allow line-crossing detection for some reason.
    SDL_Surface *buffer = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 16, 0, 0, 0, 0);

    // What we draw on the screen
    SDL_Surface *screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

    // Draw the background image
    //SDL_Surface *img = IMG_Load(background.c_str());
    //SDL_Surface *bg_img = SDL_DisplayFormat(img);
    //SDL_FreeSurface(img);
    //SDL_BlitSurface(bg_img, NULL, screen, NULL);

    if (screen == NULL || buffer == NULL)
    {
        fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
            width, height, depth, SDL_GetError());
        return 1;
    }

    // Window title and icon
	SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
	Uint32 colorkey = SDL_MapRGB(icon->format, 255,255,255);
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
	SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);
    SDL_WM_SetCaption("Sprouts", "Sprouts");

	SDL_Surface *gameOver = SDL_LoadBMP("gameOver.bmp");

    // The center of the screen
	SDL_Rect rect;
	rect.x = screen->w/2 - gameOver->w/2;
	rect.y = screen->h/2 - gameOver->h/2;

    // Game loop
	Sprout sp(screen, buffer, 3);	// create new Sprout object

	while (gameRunning)
	{
		while (gameRunning && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:					// top-right X clicked
					gameRunning = false;
					break;
				case SDL_ACTIVEEVENT:	break;	// see http://www.libsdl.org/cgi/docwiki.cgi/SDL_ActiveEvent
				case SDL_KEYDOWN:		break;	// keyboard pressed
				case SDL_KEYUP:					// keyboard released
					if (event.key.keysym.sym == SDLK_ESCAPE)
						gameRunning = false;
					else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE)
					{
						if(ended)
                        {
                            gameRunning = false;
                        }
					}
					break;
				case SDL_MOUSEBUTTONDOWN:	break;	// mouse pressed
				case SDL_MOUSEBUTTONUP:		// mouse released
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (ended)
                        {
                            gameRunning = false;
                            break;
                        }

						if (!sp.connect())		// game has ended (all nodes have degree 3 or escape pressed)
						{
							ended = true;
							SDL_BlitSurface(gameOver, NULL, screen, &rect);
							SDL_Flip(screen);
						}
					}
					break;
				case SDL_MOUSEMOTION:		// mouse moved
                    sp.highlightNear(event.motion.x, event.motion.y);
					break;
				default:
					break;
			}
		}
	}

	SDL_FreeSurface(gameOver);
    SDL_Quit();

	return 0;
}
