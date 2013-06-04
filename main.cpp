#include <cstring>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include "headers/gamegui.h"
#include "headers/image.h"

void error(const string& s)
{
    cout << "Error: " << s << endl;
}

int main(int argc,char *argv[])
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

    // Cleanup on exit
    atexit(SDL_Quit);

    // get current display information (for height, width, color depth, etc.)
	//const SDL_VideoInfo* info = SDL_GetVideoInfo();
    //int width  = info->current_w*2/3;
    //int height = info->current_h*2/3;
    int depth  = 0; // Set to current screen depth

    // What we draw on the screen
    SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

    Image instructions("images/Instructions.jpg");
    Image instructionsBackHover("images/InstructionsBackHover.jpg");
    Image credits("images/Credits.jpg");
    Image creditsBackHover("images/CreditsBackHover.jpg");
    Image exitHover("images/ExitHover.jpg");
    Image creditsHover("images/CreditsHover.jpg");
    Image instructionsHover("images/InstructionsHover.jpg");
    Image optionsHover("images/OptionsHover.jpg");
    Image startHover("images/StartHover.jpg");
    Image sproutsMenu("images/SproutsMenu.jpg");

    //SDL_Surface* chalkboard = NULL;
    //chalkboard IMG_Load( "images/background.jpg" );
    //SDL_BlitSurface(chalkboard, NULL, screen, NULL);

    SDL_BlitSurface(sproutsMenu.surface(), NULL, screen, NULL);
    SDL_Flip( screen );
    //SDL_Delay( 2000 );

    if (screen == NULL)
    {
        fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
            width, height, depth, SDL_GetError());
        return 1;
    }

    bool GameStart = false;
    bool GameOptions = false;
    bool GameInstructions = false;
    bool GameCredits = false;
    bool GameExit = false;

    bool check1 = true;
    bool check2 = false;
    bool check3 = false;
    bool check4 = false;
    bool check5 = false;
    bool check6 = false;
    bool check7 = false;
    bool check8 = false;
    bool check9 = false;
    bool check10 = false;
    bool check11 = false;
    bool check12 = false;

    // Display the menu, and when starting the game, break out of this loop and
    // start the game loop.
    while (!GameStart)
    {
        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
            if (event.type == SDL_MOUSEMOTION)
                if ((GameOptions == false) && (GameInstructions == false) && (GameCredits == false))
                {
                    if ( (event.button.y < 250) || (event.button.y > 500) )
                    {
                        check2 = check3 = check4 = check5 = check6 = false;
                        if (!check1)
                        {
                            SDL_BlitSurface(sproutsMenu.surface(), NULL, screen, NULL);
                            SDL_Flip( screen );
                            check1 = true;
                        }
                    }
                    else
                    {
                        check1 = check3 = check4 = check5 = check6 = false;
                        if ( event.button.y < 300 )
                        {
                            if (!check2)
                            {
                                SDL_BlitSurface(startHover.surface(), NULL, screen, NULL);
                                SDL_Flip( screen );
                                check2 = true;
                            }
                        }
                        else
                        {
                            check1 = check2 = check4 = check5 = check6 = false;
                            if ( event.button.y < 350 )
                            {
                                if (!check3)
                                {
                                    SDL_BlitSurface(optionsHover.surface(), NULL, screen, NULL);
                                    SDL_Flip( screen );
                                    check3 = true;
                                }
                            }
                            else
                            {
                                check1 = check2 = check3 = check5 = check6 = false;
                                if ( event.button.y < 400 )
                                {
                                    if (!check4)
                                    {
                                        SDL_BlitSurface(instructionsHover.surface(), NULL, screen, NULL);
                                        SDL_Flip( screen );
                                        check4 = true;
                                    }
                                }
                                else
                                {
                                    check1 = check2 = check3 = check4 = check6 = false;
                                    if ( event.button.y < 450 )
                                    {
                                        if (!check5)
                                        {
                                            SDL_BlitSurface(creditsHover.surface(), NULL, screen, NULL);
                                            SDL_Flip( screen );
                                            check5 = true;
                                        }
                                    }
                                    else
                                    {
                                        check1 = check2 = check3 = check4 = check5 = false;
                                        if (!check6)
                                        {
                                            SDL_BlitSurface(exitHover.surface(), NULL, screen, NULL);
                                            SDL_Flip( screen );
                                            check6 = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (GameOptions == true)
                    {

                    }
                    if (GameInstructions == true)
                    {
                        if ( event.button.y < 500 )
                        {
                            check10 = false;
                            if (!check9)
                            {
                                SDL_BlitSurface(instructions.surface(), NULL, screen, NULL);
                                SDL_Flip( screen );
                                check9 = true;
                            }
                        }
                        else
                        {
                            check9 = false;
                            if (!check10)
                            {
                                SDL_BlitSurface(instructionsBackHover.surface(), NULL, screen, NULL);
                                SDL_Flip( screen );
                                check10 = true;
                            }
                        }
                    }
                    if (GameCredits == true)
                    {
                        if ( event.button.y < 500 )
                        {
                            check12 = false;
                            if (!check11)
                            {
                                SDL_BlitSurface(credits.surface(), NULL, screen, NULL);
                                SDL_Flip( screen );
                                check11 = true;
                            }
                        }
                        else
                        {
                            check11 = false;
                            if (!check12)
                            {
                                SDL_BlitSurface(creditsBackHover.surface(), NULL, screen, NULL);
                                SDL_Flip( screen );
                                check12 = true;
                            }
                        }
                    }
                }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if ((GameOptions == false) && (GameInstructions == false) && (GameCredits == false))
                {
                    if ((event.button.y > 250) && (event.button.y < 300))
                    {
                        GameStart = true;
                    }

                    /*if ((event.button.y > 300) && (event.button.y < 350)) // Options page does not yet exist
                    {
                        GameOptions = true;
                    }*/

                    if ((event.button.y > 350) && (event.button.y < 400))
                    {
                        GameInstructions = true;
                    }

                    if ((event.button.y > 400) && (event.button.y < 450))
                    {
                        GameCredits = true;
                    }

                    if ((event.button.y > 450) && (event.button.y < 500))
                    {
                        // "Start" the game, but set the game to not running so
                        // we just exit the program
                        GameStart = true;
                        gameRunning = false;
                        break;
                    }

                    /*while (GameHighScore)
                    {

                    }*/
                }
                else
                {
                    if (event.button.y > 500)
                    {
                        GameOptions = GameInstructions = GameCredits = false;
                    }
                }
            }
        }
    }


    // Window title and icon
    /*Image icon("icon.bmp");
    SDL_WM_SetIcon(icon.surface(), NULL);*/
    SDL_WM_SetCaption("Sprouts", "Sprouts");

    // Game loop
	GameGUI game(screen, 3);	// create new Sprout object with 3 nodes

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
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            game.cancel();
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            game.click(Coord(event.button.x, event.button.y));
                        break;
                    case SDL_MOUSEMOTION:
                        game.cursor(Coord(event.motion.x, event.motion.y));
                        break;
                    default:
                        break;
                }
            }
            catch (const InvalidLine& e)
            {
                error("Tried to add an invalid line");
            }
            catch (const InvalidMove& e)
            {
                error("Tried to make an invalid move");
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
		}
	}

    SDL_Quit();

	return 0;
}
