#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "sprout.h"

int main()
{
	bool gameRunning = true, ended = false;
	SDL_Event event;	// dump event polls into this

    // Initialize everything
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
	}

    // TODO: are we going to use this?
    /*if (TTF_Init() < 0)
    {
        fprintf(stderr, "Unable to init SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }*/

    // Cleanup on exit
    //atexit(TTF_Quit);
    atexit(SDL_Quit);

    // get current display information (for height, width, color depth, etc.)
	const SDL_VideoInfo* info = SDL_GetVideoInfo();	
    int width  = info->current_w*2/3;
    int height = info->current_h*2/3;
    int depth  = 8;
    SDL_Surface *screen = SDL_SetVideoMode(width, height, depth, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (screen == NULL)
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
	SDL_Rect *rect = new SDL_Rect;
	rect->x = screen->w/2 - gameOver->w/2;
	rect->y = screen->h/2 - gameOver->h/2;

    // Game loop
	Sprout sp(screen,3);	// create new Sprout object

	while (gameRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:					// top-right X clicked
					gameRunning = false;
					break;
				case SDL_ACTIVEEVENT:	break;	// see http://www.libsdl.org/cgi/docwiki.cgi/SDL_ActiveEvent
				case SDL_KEYDOWN:		break;	// keyboard pressed
				case SDL_KEYUP:					// keyboard released
					if( event.key.keysym.sym == SDLK_ESCAPE )
						gameRunning = false;
					else if( event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE )
					{
						if( ended )
							return 0;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:	break;	// mouse pressed
				case SDL_MOUSEBUTTONUP:		// mouse released
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (ended)
							return 0;
						if (!sp.connect())		// game has ended (all nodes have degree 3 or escape pressed)
						{
							ended = true;
							SDL_BlitSurface(gameOver, NULL, screen, rect);
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

	return 0;
}
