#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include "headers/menu.h"
#include "headers/image.h"
#include "headers/gamegui.h"

void error(const string& s)
{
    cout << "Error: " << s << endl;
}

int main(int argc, char *argv[])
{
    int width = 800;
    int height = 602;

	bool gameRunning = true;
	SDL_Event event;	// dump event polls into this

    // Initialize everything
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
	}

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    // Cleanup on exit
    atexit(SDL_Quit);
    atexit(TTF_Quit);

    // What we draw on the screen
    SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (screen == NULL)
    {
        fprintf(stderr, "Couldn't set %dx%d video mode: %s\n",
            width, height, SDL_GetError());
        return 1;
    }

    // Window title and icon
    /*Image icon("icon.bmp");
    SDL_WM_SetIcon(icon.surface(), NULL);*/
    SDL_WM_SetCaption("Sprouts", "Sprouts");

    bool inMenu = true;
    Menu menu(screen);
    menu.init();
	GameGUI game(screen);

	while (gameRunning)
	{
		while (gameRunning && SDL_PollEvent(&event))
		{
            try
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
                        if (inMenu)
                            menu.cancel();
                        else
                            game.cancel();
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            // If it's in the menu, send the click event and
                            // then see if they clicked to start the game. If
                            // so, switch to game mode by setting inMenu to
                            // false and draw the game with the specified
                            // number of nodes to the screen.
                            if (inMenu)
                            {
                                ClickType status = menu.click(Coord(event.button.x, event.button.y));

                                // Exit the program.
                                if (status == EXIT)
                                {
                                    gameRunning = false;
                                }
                                // Start the game with the specified number of nodes
                                else if (status == GAME)
                                {
                                    inMenu = false;
                                    game.init(menu.nodes());
                                }
                            }
                            else
                            {
                                game.click(Coord(event.button.x, event.button.y));
                            }
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        if (inMenu)
                            menu.cursor(Coord(event.motion.x, event.motion.y));
                        else
                            game.cursor(Coord(event.motion.x, event.motion.y));
                        break;
                    default:
                        break;
                }
            }
            catch (const InvalidLine& e)
            {
                error("Tried to add an invalid line");
                cout << e << endl;
            }
            catch (const InvalidNode& e)
            {
                error("Could not find start and/or end node in the line");
            }
            catch (const InvalidMiddle& e)
            {
                error("Could not find where to place middle node in the line");
            }
            catch (const ImageNotLoaded& e)
            {
                error("Could not load image");
            }
            catch (const AreasOutdated& e)
            {
                error("Tried to find if connectable with outdated areas");
            }
            catch (const InvalidCorner& e)
            {
                error("Tried to add a middle node on a corner");
            }
            catch (const NotConnectable& e)
            {
                error("Tried to connect two nodes that shouldn't be connectable");
            }
		}

        // Don't use all the CPU
        SDL_Delay(20);
	}

    SDL_FreeSurface(screen);
    SDL_Quit();
    TTF_Quit();

	return 0;
}
