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

void Menu::menuFlip(SDL_Surface* img, bool& check)
{
    SDL_BlitSurface(img, NULL, screen, NULL);
    SDL_Flip(screen);
    check = true;
}

void Menu::init()
{
    menuFlip(sproutsMenu.surface(), check1);
}

void Menu::over(bool player1lost)
{
    GameOver = true;
    if (player1lost == false)
        menuFlip(playerOneWins.surface(), p1Wins);
    else
    {
        if (playerMode == PvP)
            menuFlip(playerTwoWins.surface(), p2Wins);
        else
            menuFlip(computerWins.surface(), aiWins);
    }
}

void Menu::cursor(Coord location)
{
    if (GameOver)
        cursorGameOver(location);
    else
        cursorMenu(location);
}

void Menu::cursorMenu(Coord location)
{
    if (GameOptions == true)
        {

        }
    else
    {
        if (GameInstructions == true)
        {
            if ( location.y < 550 )
            {
                if (!check9)
                {
                    check10 = false;
                    menuFlip(instructions.surface(), check9);
                }
            }
            else
            {
                if (!check10)
                {
                    check9 = false;
                    menuFlip(instructionsBackHover.surface(), check10);
                }
            }
        }
        else
        {
            if (GameCredits == true)
            {
                if ( location.y < 550 )
                {
                    if (!check11)
                    {
                        check12 = false;
                        menuFlip(credits.surface(), check11);
                    }
                }
                else
                {
                    if (!check12)
                    {
                        check11 = false;
                        menuFlip(creditsBackHover.surface(), check12);
                    }
                }
            }
            else
            {
                if ( (location.y < 250) || (location.y > 500) )
                {
                    if (!check1)
                    {
                        check2 = check3 = check4 = check5 = check6 = false;
                        menuFlip(sproutsMenu.surface(), check1);
                    }
                }
                else
                {
                    if ( location.y < 300 )
                    {
                        if (!check2)
                        {
                            check1 = check3 = check4 = check5 = check6 = false;
                            menuFlip(startHover.surface(), check2);
                        }
                    }
                    else
                    {
                        if ( location.y < 350 )
                        {
                            if (!check3)
                            {
                                check1 = check2 = check4 = check5 = check6 = false;
                                menuFlip(optionsHover.surface(), check3);
                            }
                        }
                        else
                        {
                            if ( location.y < 400 )
                            {
                                if (!check4)
                                {
                                    check1 = check2 = check3 = check5 = check6 = false;
                                    menuFlip(instructionsHover.surface(), check4);
                                }
                            }
                            else
                            {
                                if ( location.y < 450 )
                                {
                                    if (!check5)
                                    {
                                        check1 = check2 = check3 = check4 = check6 = false;
                                        menuFlip(creditsHover.surface(), check5);
                                    }
                                }
                                else
                                {
                                    if (!check6)
                                    {
                                        check1 = check2 = check3 = check4 = check5 = false;
                                        menuFlip(exitHover.surface(), check6);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Menu::cursorGameOver(Coord location)
{
    if (p1Wins == true)
    {
        if ( (location.y < 390) || (location.y > 490) )
        {
            if (!check13)
            {
                check14 = check15 = false;
                menuFlip(playerOneWins.surface(), check13);
            }
        }
        else
        {
            if ( location.y < 440 )
            {
                if (!check14)
                {
                    check13 = check15 = false;
                    menuFlip(playerOneWinsMenuHover.surface(), check14);
                }
            }
            else
            {
                if ( location.y < 490 )
                {
                    if (!check15)
                    {
                        check13 = check14 = false;
                        menuFlip(playerOneWinsExitHover.surface(), check15);
                    }
                }
            }
        }
    }
    else
    {
        if (p2Wins == true)
        {
            if ( (location.y < 390) || (location.y > 490) )
            {
                if (!check16)
                {
                    check17 = check18 = false;
                    menuFlip(playerTwoWins.surface(), check16);
                }
            }
            else
            {
                if ( location.y < 440 )
                {
                    if (!check17)
                    {
                        check16 = check18 = false;
                        menuFlip(playerTwoWinsMenuHover.surface(), check17);
                    }
                }
                else
                {
                    if ( location.y < 490 )
                    {
                        if (!check18)
                        {
                            check16 = check17 = false;
                            menuFlip(playerTwoWinsExitHover.surface(), check18);
                        }
                    }
                }
            }
        }
        else
        {
            if ( (location.y < 390) || (location.y > 490) )
            {
                if (!check19)
                {
                    check20 = check21 = false;
                    menuFlip(computerWins.surface(), check19);
                }
            }
            else
            {
                if ( location.y < 440 )
                {
                    if (!check20)
                    {
                        check19 = check21 = false;
                        menuFlip(computerWinsMenuHover.surface(), check20);
                    }
                }
                else
                {
                    if ( location.y < 490 )
                    {
                        if (!check21)
                        {
                            check19 = check20 = false;
                            menuFlip(computerWinsExitHover.surface(), check21);
                        }
                    }
                }
            }
        }
    }
}

ClickType Menu::click(Coord location)
{
    if (GameOver)
        return clickGameOver(location);
    else
        return clickMenu(location);
}

ClickType Menu::clickMenu(Coord location)
{
    if ((GameOptions == false) && (GameInstructions == false) && (GameCredits == false))
    {
        if (location.y > 250)
        {
            if (location.y < 300)
            {
                GameStart = true;
                return GAME;
            }
            else
            {
                if (location.y < 350)
                {
                    //GameOptions = true;
                    //menuFlip(options.surface(), check7);
                    //check8 = false;
                }
                else
                {
                    if (location.y < 400)
                    {
                        GameInstructions = true;
                        menuFlip(instructions.surface(), check9);
                        check10 = false;
                    }
                    else
                    {
                        if (location.y < 450)
                        {
                            GameCredits = true;
                            menuFlip(credits.surface(), check11);
                            check12 = false;
                        }
                        else
                        {
                            if (location.y < 500)
                            {
                                // "Start" the game, but set the game to not running so
                                // we just exit the program
                                GameStart = true;

                                // Return EXIT
                                return EXIT;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (location.y > 550)
        {
            GameOptions = GameInstructions = GameCredits = false;
            menuFlip(sproutsMenu.surface(), check1);
        }
    }
    return CONTINUE;
}

ClickType Menu::clickGameOver(Coord location)
{
    if ((location.y > 390) && (location.y <490))
    {
        if (location.y < 440)
        {
            GameOver = p1Wins = p2Wins = aiWins = false;
            init();
        }
        else
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
