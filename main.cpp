#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "headers/gamegui.h"
#include "headers/image.h"

int main(int argc,char *argv[])
{
    int width = 800;
    int height = 602;
    std::string background = "images/background.jpg";

	bool gameRunning = true;
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
    /*Image icon("icon.bmp");
    SDL_WM_SetIcon(icon.surface(), NULL);
    SDL_WM_SetCaption("Sprouts", "Sprouts");*/

    // Game loop
	GameGUI game(screen, 3);	// create new Sprout object with 3 nodes

	while (gameRunning)
	{
		while (gameRunning && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: // Alt+F4, X, ...
					gameRunning = false;
					break;
                case SDL_ACTIVEEVENT:
                    // see http://www.libsdl.org/cgi/docwiki.cgi/SDL_ActiveEvent
                    break;
				case SDL_KEYDOWN:
                    break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_ESCAPE)
                        game.cancel();
					break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
                        game.click(event.button.x, event.button.y);
					break;
				case SDL_MOUSEMOTION:
                    game.cursor(event.motion.x, event.motion.y);
					break;
				default:
					break;
			}
		}
	}

    SDL_Quit();

	return 0;
}
