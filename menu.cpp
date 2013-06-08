#include "headers/menu.h"
#include "headers/gamegui.h"

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

      computerWins("images/ComputerWins.jpg"),
      computerWinsExitHover("images/ComputerWinsExitHover.jpg"),
      computerWinsMenuHover("images/ComputerWinsMenuHover.jpg"),
      playerOneWins("images/PlayerOneWins.jpg"),
      playerOneWinsExitHover("images/PlayerOneWinsExitHover.jpg"),
      playerOneWinsMenuHover("images/PlayerOneWinsMenuHover.jpg"),
      playerTwoWins("images/PlayerTwoWins.jpg"),
      playerTwoWinsExitHover("images/PlayerTwoWinsExitHover.jpg"),
      playerTwoWinsMenuHover("images/PlayerTwoWinsMenuHover.jpg"),

      GameStart(false),
      GameOptions(false),
      GameInstructions(false),
      GameCredits(false),
      GameExit(false),
      GameOver(false),
      p1Wins(false),
      p2Wins(false),
      aiWins(false),

      check1(false), check2(false),  check3(false),  check4(false),
      check5(false), check6(false),  check7(false),  check8(false),
      check9(false), check10(false), check11(false), check12(false),
      check13(false), check14(false), check15(false), check16(false),
      check17(false), check18(false), check19(false), check20(false),
      check21(false)
{

}

void Menu::init()
{
    SDL_BlitSurface(sproutsMenu.surface(), NULL, screen, NULL);
    SDL_Flip(screen);
    check1 = true;
}

void Menu::over(bool player1lost)
{
    GameOver = true;
    if (player1lost == false)
    {
        SDL_BlitSurface(playerOneWins.surface(), NULL, screen, NULL);
        SDL_Flip(screen);
        p1Wins = true;
    }
    else
    {
        if (playerMode == PvP)
        {
            SDL_BlitSurface(playerTwoWins.surface(), NULL, screen, NULL);
            SDL_Flip(screen);
            p2Wins = true;
        }
        else
        {
            SDL_BlitSurface(computerWins.surface(), NULL, screen, NULL);
            SDL_Flip(screen);
            aiWins = true;
        }
    }
}

void Menu::cursor(Coord location)
{
    if (GameOver == false)
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
                if ( location.y < 550 )
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
                if ( location.y < 550 )
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
    else
    {
        if (p1Wins == true)
        {
            if ( (location.y < 390) || (location.y > 490) )
            {
                check14 = check15 = false;
                if (!check13)
                {
                    SDL_BlitSurface(playerOneWins.surface(), NULL, screen, NULL);
                    SDL_Flip( screen );
                    check13 = true;
                }
            }
            else
            {
                check13 = check15 = false;
                if ( location.y < 440 )
                {
                    if (!check14)
                    {
                        SDL_BlitSurface(playerOneWinsMenuHover.surface(), NULL, screen, NULL);
                        SDL_Flip( screen );
                        check14 = true;
                    }
                }
                else
                {
                    check13 = check14 = false;
                    if ( location.y < 490 )
                    {
                        if (!check15)
                        {
                            SDL_BlitSurface(playerOneWinsExitHover.surface(), NULL, screen, NULL);
                            SDL_Flip( screen );
                            check15 = true;
                        }
                    }
                }
            }
        }
        if (p2Wins == true)
        {
            if ( (location.y < 390) || (location.y > 490) )
            {
                check17 = check18 = false;
                if (!check16)
                {
                    SDL_BlitSurface(playerTwoWins.surface(), NULL, screen, NULL);
                    SDL_Flip( screen );
                    check16 = true;
                }
            }
            else
            {
                check16 = check18 = false;
                if ( location.y < 440 )
                {
                    if (!check17)
                    {
                        SDL_BlitSurface(playerTwoWinsMenuHover.surface(), NULL, screen, NULL);
                        SDL_Flip( screen );
                        check17 = true;
                    }
                }
                else
                {
                    check16 = check17 = false;
                    if ( location.y < 490 )
                    {
                        if (!check18)
                        {
                            SDL_BlitSurface(playerTwoWinsExitHover.surface(), NULL, screen, NULL);
                            SDL_Flip( screen );
                            check18 = true;
                        }
                    }
                }
            }
        }
        if (aiWins == true)
        {
            if ( (location.y < 390) || (location.y > 490) )
            {
                check20 = check21 = false;
                if (!check19)
                {
                    SDL_BlitSurface(computerWins.surface(), NULL, screen, NULL);
                    SDL_Flip( screen );
                    check19 = true;
                }
            }
            else
            {
                check19 = check21 = false;
                if ( location.y < 440 )
                {
                    if (!check20)
                    {
                        SDL_BlitSurface(computerWinsMenuHover.surface(), NULL, screen, NULL);
                        SDL_Flip( screen );
                        check20 = true;
                    }
                }
                else
                {
                    check19 = check20 = false;
                    if ( location.y < 490 )
                    {
                        if (!check21)
                        {
                            SDL_BlitSurface(computerWinsExitHover.surface(), NULL, screen, NULL);
                            SDL_Flip( screen );
                            check21 = true;
                        }
                    }
                }
            }
        }
    }
}

ClickType Menu::click(Coord location)
{
    if (GameOver == false)
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
            if (location.y > 550)
            {
                GameOptions = GameInstructions = GameCredits = false;
                SDL_BlitSurface(sproutsMenu.surface(), NULL, screen, NULL);
                SDL_Flip(screen);
                check1 = true;
            }
        }
    }
    else
    {
        if ( (location.y > 390) && (location.y < 440) )
        {
            GameOver = false;
            init();
        }
        if ( (location.y > 440) && (location.y < 490) )
        {
            // "Start" the game, but set the game to not running so
                // we just exit the program
                GameStart = true;

                // Return EXIT
                return EXIT;
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
