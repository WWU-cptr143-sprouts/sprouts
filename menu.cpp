#include "headers/menu.h"

Menu::Menu(SDL_Surface* screen)
    : screen(screen),
      player1Name("Player 1"),
      player2Name("Player 2"),
      playerMode(PvP),
      numberOfNodes(3),
      instructions("images/Instructions.jpg"),
      instructionsBackHover("images/InstructionsBackHover.jpg"),
      credits("images/Credits.jpg"),
      creditsBackHover("images/CreditsBackHover.jpg"),
      exitHover("images/ExitHover.jpg"),
      creditsHover("images/CreditsHover.jpg"),
      instructionsHover("images/InstructionsHover.jpg"),
      optionsHover("images/OptionsHover.jpg"),
      startHover("images/StartHover.jpg"),
      sproutsMenu("images/SproutsMenu.jpg"),
      GameStart(false),
      GameOptions(false),
      GameInstructions(false),
      GameCredits(false),
      GameExit(false),
      check1(false), check2(false),  check3(false),  check4(false),
      check5(false), check6(false),  check7(false),  check8(false),
      check9(false), check10(false), check11(false), check12(false)
{

}

void Menu::init()
{
    SDL_BlitSurface(sproutsMenu.surface(), NULL, screen, NULL);
    SDL_Flip(screen);
}

void Menu::cursor(Coord location)
{
    if ((GameOptions == false) && (GameInstructions == false) && (GameCredits == false))
    {
        if ( (location.y < 250) || (location.y > 500) )
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
            if ( location.y < 300 )
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
                if ( location.y < 350 )
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
                    if ( location.y < 400 )
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
                        if ( location.y < 450 )
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
            if ( location.y < 500 )
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
            if ( location.y < 500 )
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
}

ClickType Menu::click(Coord location)
{
    if ((GameOptions == false) && (GameInstructions == false) && (GameCredits == false))
    {
        if ((location.y > 250) && (location.y < 300))
        {
            GameStart = true;
            return GAME;
        }

        /*if ((location.y > 300) && (location.y < 350)) // Options page does not yet exist
         {
            GameOptions = true;
         }*/

        if ((location.y > 350) && (location.y < 400))
        {
            GameInstructions = true;
            SDL_BlitSurface(instructions.surface(), NULL, screen, NULL);
            SDL_Flip( screen );
            check9 = true;
        }

        if ((location.y > 400) && (location.y < 450))
        {
            GameCredits = true;
            SDL_BlitSurface(credits.surface(), NULL, screen, NULL);
            SDL_Flip( screen );
            check11 = true;
        }

        if ((location.y > 450) && (location.y < 500))
        {
            // "Start" the game, but set the game to not running so
            // we just exit the program
            GameStart = true;

            // Return EXIT
            return EXIT;
        }

        /*while (GameHighScore)
         {

         }*/
    }
    else
    {
        if (location.y > 500)
        {
            GameOptions = GameInstructions = GameCredits = false;
            SDL_BlitSurface(sproutsMenu.surface(), NULL, screen, NULL);
            SDL_Flip(screen);
            check1 = true;
        }
    }

    return CONTINUE;
}

void Menu::cancel()
{

}

Menu::~Menu()
{

}
