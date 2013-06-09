#include "headers/menu.h"

Menu::Menu(SDL_Surface* screen)
: screen(screen),
optionsNodeRadius(5),
optionsLineThick(1),
optionsSelectRadius(10),
playerMode(PvAI),
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
aiWins(false)/*,

check1(false), check2(false),  check3(false),  check4(false),
check5(false), check6(false),  check7(false),  check8(false),
check9(false), check10(false), check11(false), check12(false),
check13(false), check14(false), check15(false), check16(false),
check17(false), check18(false), check19(false), check20(false),
check21(false)*/
{
    fill(&check[0], &check[0]+37, false);
}

void Menu::menuFlip(SDL_Surface* img, bool& check)
{
    SDL_BlitSurface(img, NULL, screen, NULL);
    SDL_Flip(screen);
    check = true;
}

void Menu::init()
{
    menuFlip(sproutsMenu.surface(), check[1]);
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
        optionsPageCursor(location);
    }
    else
    {
        if (GameInstructions == true)
        {
            if ( location.y < 550 )
            {
                if (!check[9])
                {
                    check[10] = false;
                    menuFlip(instructions.surface(), check[9]);
                }
            }
            else
            {
                if (!check[10])
                {
                    check[9] = false;
                    menuFlip(instructionsBackHover.surface(), check[10]);
                }
            }
        }
        else
        {
            if (GameCredits == true)
            {
                if ( location.y < 550 )
                {
                    if (!check[11])
                    {
                        check[12] = false;
                        menuFlip(credits.surface(), check[11]);
                    }
                }
                else
                {
                    if (!check[12])
                    {
                        check[11] = false;
                        menuFlip(creditsBackHover.surface(), check[12]);
                    }
                }
            }
            else
            {
                if ( (location.y < 250) || (location.y > 500) )
                {
                    if (!check[1])
                    {
                        for (int i=1; i<=6; i++)
                        {
                            check[i] = false;
                        }
                        menuFlip(sproutsMenu.surface(), check[1]);
                    }
                }
                else
                {
                    if ( location.y < 300 )
                    {
                        if (!check[2])
                        {
                            for (int i=1; i<=6; i++)
                            {
                                check[i] = false;
                            }
                            menuFlip(startHover.surface(), check[2]);
                        }
                    }
                    else
                    {
                        if ( location.y < 350 )
                        {
                            if (!check[3])
                            {
                                for (int i=1; i<=6; i++)
                                {
                                    check[i] = false;
                                }
                                menuFlip(optionsHover.surface(), check[3]);
                            }
                        }
                        else
                        {
                            if ( location.y < 400 )
                            {
                                if (!check[4])
                                {
                                    for (int i=1; i<=6; i++)
                                    {
                                        check[i] = false;
                                    }
                                    menuFlip(instructionsHover.surface(), check[4]);
                                }
                            }
                            else
                            {
                                if ( location.y < 450 )
                                {
                                    if (!check[5])
                                    {
                                        for (int i=1; i<=6; i++)
                                        {
                                            check[i] = false;
                                        }
                                        menuFlip(creditsHover.surface(), check[5]);
                                    }
                                }
                                else
                                {
                                    if (!check[6])
                                    {
                                        for (int i=1; i<=6; i++)
                                        {
                                            check[i] = false;
                                        }
                                        menuFlip(exitHover.surface(), check[6]);
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
            if (!check[13])
            {
                check[14] = check[15] = false;
                menuFlip(playerOneWins.surface(), check[13]);
            }
        }
        else
        {
            if ( location.y < 440 )
            {
                if (!check[14])
                {
                    check[13] = check[15] = false;
                    menuFlip(playerOneWinsMenuHover.surface(), check[14]);
                }
            }
            else
            {
                if ( location.y < 490 )
                {
                    if (!check[15])
                    {
                        check[13] = check[14] = false;
                        menuFlip(playerOneWinsExitHover.surface(), check[15]);
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
                if (!check[16])
                {
                    check[17] = check[18] = false;
                    menuFlip(playerTwoWins.surface(), check[16]);
                }
            }
            else
            {
                if ( location.y < 440 )
                {
                    if (!check[17])
                    {
                        check[16] = check[18] = false;
                        menuFlip(playerTwoWinsMenuHover.surface(), check[17]);
                    }
                }
                else
                {
                    if ( location.y < 490 )
                    {
                        if (!check[18])
                        {
                            check[16] = check[17] = false;
                            menuFlip(playerTwoWinsExitHover.surface(), check[18]);
                        }
                    }
                }
            }
        }
        else
        {
            if ( (location.y < 390) || (location.y > 490) )
            {
                if (!check[19])
                {
                    check[20] = check[21] = false;
                    menuFlip(computerWins.surface(), check[19]);
                }
            }
            else
            {
                if ( location.y < 440 )
                {
                    if (!check[20])
                    {
                        check[19] = check[21] = false;
                        menuFlip(computerWinsMenuHover.surface(), check[20]);
                    }
                }
                else
                {
                    if ( location.y < 490 )
                    {
                        if (!check[21])
                        {
                            check[19] = check[20] = false;
                            menuFlip(computerWinsExitHover.surface(), check[21]);
                        }
                    }
                }
            }
        }
    }
}

void Menu::optionsPageCursor(Coord location)
{
    /*if ( (location.y < 250) || (location.y > 500) || (location.x < 100) || (location.x > 600))
    {
        if (!check[22])
        {
            for (int i=22; i<=36; i++)
            {
                check[i] = false;
            }
            //menuFlip(options.surface(), check[22]);
        }
    }
    else
    {
        if ( location.y < 300 )
        {
            if (location.x < 200)
            {
                if (!check[23])
                {
                    for (int i=22; i<=36; i++)
                    {
                        check[i] = false;
                    }
                    //menuFlip(optionsSmallNodeHover.surface(), check[23]);
                }
            }
            else
            {
                if (location.x < 300)
                {
                    if (!check[24])
                    {
                        for (int i=22; i<=36; i++)
                        {
                            check[i] = false;
                        }
                        //menuFlip(optionsMediumNodeHover.surface(), check[24]);
                    }
                }
                else
                {
                    if (location.x < 400)
                    {
                        if (!check[25])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            //menuFlip(optionsLargeNodeHover.surface(), check[25]);
                        }
                    }
                }
            }
        }
        else
        {
            if ( location.y < 350 )
            {
                if (location.x < 200)
                {
                    if (!check[26])
                    {
                        for (int i=22; i<=36; i++)
                        {
                            check[i] = false;
                        }
                        //menuFlip(optionsSmallLineHover.surface(), check[26]);
                    }
                }
                else
                {
                    if (location.x < 300)
                    {
                        if (!check[27])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            //menuFlip(optionsMediumLineHover.surface(), check[27]);
                        }
                    }
                    else
                    {
                        if (location.x < 400)
                        {
                            if (!check[28])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                //menuFlip(optionsLargeLineHover.surface(), check[28]);
                            }
                        }
                    }
                }
            }
            else
            {
                if ( location.y < 400 )
                {
                    if (location.x < 200)
                    {
                        if (!check[29])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            //menuFlip(optionsThreeNodesHover.surface(), check[29]);
                        }
                    }
                    else
                    {
                        if (location.x < 300)
                        {
                            if (!check[30])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                //menuFlip(optionsFourNodesHover.surface(), check[30]);
                            }
                        }
                        else
                        {
                            if (location.x < 400)
                            {
                                if (!check[31])
                                {
                                    for (int i=22; i<=36; i++)
                                    {
                                        check[i] = false;
                                    }
                                    //menuFlip(optionsFiveNodesHover.surface(), check[31]);
                                }
                            }
                            else
                            {
                                if (location.x < 500)
                                {
                                    if (!check[32])
                                    {
                                        for (int i=22; i<=36; i++)
                                        {
                                            check[i] = false;
                                        }
                                        //menuFlip(optionsSixNodesHover.surface(), check[32]);
                                    }
                                }
                                else
                                {
                                    if (location.x < 600)
                                    {
                                        if (!check[33])
                                        {
                                            for (int i=22; i<=36; i++)
                                            {
                                                check[i] = false;
                                            }
                                            //menuFlip(optionsSevenNodesHover.surface(), check[33]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if ( location.y < 450 )
                    {
                        if (location.x < 200)
                        {
                            if (!check[34])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                //menuFlip(optionsPvPHover.surface(), check[34]);
                            }
                        }
                        else
                        {
                            if (!check[35])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                //menuFlip(optionsPvAIHover.surface(), check[35]);
                            }
                        }
                    }
                    else
                    {
                        if (!check[36])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            //menuFlip(optionsExitHover.surface(), check[36]);
                        }
                    }
                }
            }
        }
    }*/
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
                    GameOptions = true;
                    //menuFlip(options.surface(), GameOptions);
                }
                else
                {
                    if (location.y < 400)
                    {
                        GameInstructions = true;
                        menuFlip(instructions.surface(), check[9]);
                        check[10] = false;
                    }
                    else
                    {
                        if (location.y < 450)
                        {
                            GameCredits = true;
                            menuFlip(credits.surface(), check[11]);
                            check[12] = false;
                        }
                        else
                        {
                            if (location.y < 500)
                            {
                                // Return EXIT to end the game
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
        if (GameOptions == true)
        {
            optionsPageClick(location);
        }
        else
        {
            if (location.y > 550)
            {
                GameInstructions = GameCredits = false;
                menuFlip(sproutsMenu.surface(), check[1]);
            }
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
            // Return EXIT to end the game
            return EXIT;
        }
    }
    return CONTINUE;
}

void Menu::optionsPageClick(Coord location)
{
    /*if ((location.y > 250) || (location.y < 500) || (location.x > 100) || (location.x < 600))
    {
        if ( location.y < 300 )
        {
            if (location.x < 200)
            {
                optionsNodeRadius = 5;
                optionsSelectRadius = 10;
            }
            else
            {
                if (location.x < 300)
                {
                    optionsNodeRadius = 10;
                    optionsSelectRadius = 10;
                }
                else
                {
                    if (location.x < 400)
                    {
                        optionsNodeRadius = 15;
                        optionsSelectRadius = 15;
                    }
                }
            }
        }
        else
        {
            if ( location.y < 350 )
            {
                if (location.x < 200)
                {
                    optionsLineThick = 1;
                }
                else
                {
                    if (location.x < 300)
                    {
                        optionsLineThick = 2;
                    }
                    else
                    {
                        if (location.x < 400)
                        {
                            optionsLineThick = 3;
                        }
                    }
                }
            }
            else
            {
                if ( location.y < 400 )
                {
                    if (location.x < 200)
                    {
                        numberOfNodes = 3;
                    }
                    else
                    {
                        if (location.x < 300)
                        {
                            numberOfNodes = 4;
                        }
                        else
                        {
                            if (location.x < 400)
                            {
                                numberOfNodes = 5;
                            }
                            else
                            {
                                if (location.x < 500)
                                {
                                    numberOfNodes = 6;
                                }
                                else
                                {
                                    if (location.x < 600)
                                    {
                                        numberOfNodes = 7;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if ( location.y < 450 )
                    {
                        if (location.x < 200)
                        {
                            playerMode = PvP;
                        }
                        else
                        {
                            playerMode = PvAI;
                        }
                    }
                    else
                    {
                        GameOptions = false;
                        menuFlip(sproutsMenu.surface(), check[1]);
                    }
                }
            }
        }
    }*/
}

void Menu::cancel()
{

}

Menu::~Menu()
{

}
