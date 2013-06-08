#include <csignal>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include "headers/menu.h"
#include "headers/image.h"
#include "headers/gamegui.h"

int main(int argc, char *argv[])
{
    int width = 800;
    int height = 602;

	bool gameRunning = true;
	SDL_Event event;	// dump event polls into this

    // Initialize SDL, NOPARACHUTE means SDL will handle fatal interrupts which
    // we use to exit on Ctrl+C.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
	{
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
	}

    // Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    // Exit on Ctrl+C
    signal(SIGINT, SIG_DFL);

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
    Image icon("images/icon1.bmp", true);
    //Image icon("images/icon2.bmp", true); // a second possible icon
    SDL_WM_SetIcon(icon.surface(), NULL);
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
                        break;
                    case SDL_KEYDOWN:
                        break;
                    case SDL_KEYUP:
                        // Escape cancels line and does nothing (at the moment) in the menu
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            if (inMenu)
                                menu.cancel();
                            else
                                game.cancel();
                        }
                        // Q returns to menu
                        else if (event.key.keysym.sym == SDLK_q && !inMenu)
                        {
                            inMenu = true;
                            menu.init();
                        }
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
                                State state = game.click(Coord(event.button.x, event.button.y));

                                if (state == GameEnd)
                                {
                                    cout << "Game has ended!" << endl; //Add image here for end game
                                    game = GameGUI(screen);
                                    inMenu = true;
                                    //menu.init();
                                    menu.over(game.playerTurn());
                                }
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
            // Catch all of the errors that might have occurred in the game. If
            // these are thrown, there's a GUI problem since the GUI shouldn't
            // allow invalid moves. That, or the A-Checker has a bug that saying
            // a move is invalid when it shouldn't be.
            catch (const InvalidLine& e)
            {
                cout << "Error: Tried to add an invalid line. " << e << endl;
            }
            catch (const InvalidNode& e)
            {
                cout << "Error: Could not find start and/or end node in the line" << endl;
            }
            catch (const InvalidMiddle& e)
            {
                cout << "Error: Could not find where to place middle node in the line. " << e << endl;
            }
            catch (const ImageNotLoaded& e)
            {
                cout << "Error: Could not load image" << endl;
            }
            catch (const AreasOutdated& e)
            {
                cout << "Error: Tried to find if connectable with outdated areas" << endl;
            }
            catch (const InvalidCorner& e)
            {
                cout << "Error: Tried to add a middle node on a corner" << endl;
            }
            // Note that NotConnectable is only thrown when Game game(true), setting extraChecks to true
            catch (const NotConnectable& e)
            {
                cout << "Error: Tried to connect two nodes that shouldn't be connectable" << endl;
            }
            catch (...)
            {
                cout << "Error: Unhandled Exception" << endl;
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
