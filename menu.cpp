// This is a very basic menu system which could stand for a lot of improvement

#include "headers/menu.h"

Menu::Menu(SDL_Surface* screen)
: screen(screen),
optionsNodeRadius(5), // the drawn node radius is set to 5 by default
optionsLineThick(1), // the line thickness is set to 1 by default
playerMode(PvP), // the player mode is set to two player by default
numberOfNodes(3), // the number of nodes is set to 3 by default

// the following lines of code load the images to the appropriate Image class variables
sproutsMenu("images/SproutsMenu.jpg"),
startHover("images/StartHover.jpg"),
optionsHover("images/OptionsHover.jpg"),
instructionsHover("images/InstructionsHover.jpg"),
creditsHover("images/CreditsHover.jpg"),
exitHover("images/ExitHover.jpg"),

options("images/Options.jpg"),
optionsSmallNodeHover("images/optionsSmallNodeHover.jpg"),
optionsMediumNodeHover("images/optionsMediumNodeHover.jpg"),
optionsLargeNodeHover("images/optionsLargeNodeHover.jpg"),
optionsSmallLineHover("images/optionsSmallLineHover.jpg"),
optionsMediumLineHover("images/optionsMediumLineHover.jpg"),
optionsLargeLineHover("images/optionsLargeLineHover.jpg"),
optionsThreeNodesHover("images/optionsThreeNodesHover.jpg"),
optionsFourNodesHover("images/optionsFourNodesHover.jpg"),
optionsFiveNodesHover("images/optionsFiveNodesHover.jpg"),
optionsSixNodesHover("images/optionsSixNodesHover.jpg"),
optionsSevenNodesHover("images/optionsSevenNodesHover.jpg"),
optionsPvPHover("images/optionsPvPHover.jpg"),
optionsPvAIHover("images/optionsPvAIHover.jpg"),
optionsBackHover("images/optionsBackHover.jpg"),

instructions("images/Instructions.jpg"),
instructionsBackHover("images/InstructionsBackHover.jpg"),

credits("images/Credits.jpg"),
creditsBackHover("images/CreditsBackHover.jpg"),

computerWins("images/ComputerWins.jpg"),
computerWinsExitHover("images/ComputerWinsExitHover.jpg"),
computerWinsMenuHover("images/ComputerWinsMenuHover.jpg"),
playerOneWins("images/PlayerOneWins.jpg"),
playerOneWinsExitHover("images/PlayerOneWinsExitHover.jpg"),
playerOneWinsMenuHover("images/PlayerOneWinsMenuHover.jpg"),
playerTwoWins("images/PlayerTwoWins.jpg"),
playerTwoWinsExitHover("images/PlayerTwoWinsExitHover.jpg"),
playerTwoWinsMenuHover("images/PlayerTwoWinsMenuHover.jpg"),

// different pages are shown based on which variable is flipped to true
GameStart(false),
GameOptions(false),
GameInstructions(false),
GameCredits(false),
GameExit(false),
GameOver(false),
p1Wins(false),
p2Wins(false),
aiWins(false)
{
    fill(&check[0], &check[0]+37, false);
}

// the following function is used to change the look of the screen and to flip a bool which prevents it being flipped again unnecessarily
void Menu::menuFlip(SDL_Surface* img, bool& check)
{
    SDL_BlitSurface(img, NULL, screen, NULL);
    SDL_Flip(screen);
    check = true;
}

// the following function makes the background the main menu
void Menu::init()
{
    menuFlip(sproutsMenu.surface(), check[1]);
}

// the following function tells the rest of the menu that the game is over and tells it which game over page to display
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

// the following function tells the program whether it should should call the main menu or game over menu
void Menu::cursor(Coord location)
{
    if (GameOver)
        cursorGameOver(location);
    else
        cursorMenu(location);
}

// this combines with the switch in the main.cpp to change the background effectively highlighting different buttons based on where the user moves the mouse
void Menu::cursorMenu(Coord location)
{
    if (GameOptions == true) // the options button is clicked on calling the options page
    {
        optionsPageCursor(location);
    }
    else
    {
        if (GameInstructions == true)// the instructions button is clicked on calling the options page
        {
            if ( location.y < 550 ) // the back button is not highlighted
            {
                if (!check[9]) // the check is false then the backgrond is switched and the check is made true so the background won't change again until
                               // the mouse is moved over or away from another button
                {
                    check[10] = false;
                    menuFlip(instructions.surface(), check[9]);
                }
            }
            else // the back button is highlighted
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
            if (GameCredits == true) // the credits button is clicked on calling the credits page
            {
                if ( location.y < 550 ) // the back button is not highlighted
                {
                    if (!check[11])
                    {
                        check[12] = false;
                        menuFlip(credits.surface(), check[11]);
                    }
                }
                else // the back button is highlighted
                {
                    if (!check[12])
                    {
                        check[11] = false;
                        menuFlip(creditsBackHover.surface(), check[12]);
                    }
                }
            }
            else // you are simply moving the mouse around the main menu page
            {
                if ( (location.y < 250) || (location.y > 500) ) // if the mouse coordinates fit this description then no buttons are highlighted
                {
                    if (!check[1])
                    {
                        for (int i=1; i<=6; i++)
                        {
                            check[i] = false;
                        }
                        menuFlip(sproutsMenu.surface(), check[1]); // no buttons are highlighted
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
                            menuFlip(startHover.surface(), check[2]); // the start button is highlighted
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
                                menuFlip(optionsHover.surface(), check[3]); // the options button is highlighted
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
                                    menuFlip(instructionsHover.surface(), check[4]); // the instructions button is highlighted
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
                                        menuFlip(creditsHover.surface(), check[5]); // the credits button is highlighted
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
                                        menuFlip(exitHover.surface(), check[6]); // the exit button is highlighted
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
    if (p1Wins == true) // a system that shows who won the game based on the bool that is flipped to true based on who moved last
    {
        if ( (location.y < 390) || (location.y > 490) )
        {
            if (!check[13])
            {
                check[14] = check[15] = false;
                menuFlip(playerOneWins.surface(), check[13]); // no buttons are highlighted
            }
        }
        else
        {
            if ( location.y < 440 )
            {
                if (!check[14])
                {
                    check[13] = check[15] = false;
                    menuFlip(playerOneWinsMenuHover.surface(), check[14]); // the menu button is highlighted
                }
            }
            else
            {
                if ( location.y < 490 )
                {
                    if (!check[15])
                    {
                        check[13] = check[14] = false;
                        menuFlip(playerOneWinsExitHover.surface(), check[15]); // the exit button is highlighted
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
                    menuFlip(playerTwoWins.surface(), check[16]); // no button is highlighted
                }
            }
            else
            {
                if ( location.y < 440 )
                {
                    if (!check[17])
                    {
                        check[16] = check[18] = false;
                        menuFlip(playerTwoWinsMenuHover.surface(), check[17]); // the menu button is highlighted
                    }
                }
                else
                {
                    if ( location.y < 490 )
                    {
                        if (!check[18])
                        {
                            check[16] = check[17] = false;
                            menuFlip(playerTwoWinsExitHover.surface(), check[18]); // the exit button is highlighted
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
                    menuFlip(computerWins.surface(), check[19]); // no button is highlighted
                }
            }
            else
            {
                if ( location.y < 440 )
                {
                    if (!check[20])
                    {
                        check[19] = check[21] = false;
                        menuFlip(computerWinsMenuHover.surface(), check[20]); // the menu button is highlighted
                    }
                }
                else
                {
                    if ( location.y < 500 )
                    {
                        if (!check[21])
                        {
                            check[19] = check[20] = false;
                            menuFlip(computerWinsExitHover.surface(), check[21]); // the exit button is highlighted
                        }
                    }
                }
            }
        }
    }
}

void Menu::optionsPageCursor(Coord location) // highlights the different buttons of the options page based on the mouse location
{
    if ( (location.y < 200) || (location.y > 575) || (location.x < 345) || (location.x > 700))
    {
        if (!check[22])
        {
            for (int i=22; i<=36; i++)
            {
                check[i] = false;
            }
            menuFlip(options.surface(), check[22]); // no button is highlighted
        }
    }
    else
    {
        if ( location.y < 260 )
        {
            if (location.x < 460)
            {
                if (!check[23])
                {
                    for (int i=22; i<=36; i++)
                    {
                        check[i] = false;
                    }
                    menuFlip(optionsSmallNodeHover.surface(), check[23]); // the small node button is highlighted
                }
            }
            else
            {
                if (location.x < 600)
                {
                    if (!check[24])
                    {
                        for (int i=22; i<=36; i++)
                        {
                            check[i] = false;
                        }
                        menuFlip(optionsMediumNodeHover.surface(), check[24]); // the medium node button is highlighted
                    }
                }
                else
                {
                    if (location.x < 700)
                    {
                        if (!check[25])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            menuFlip(optionsLargeNodeHover.surface(), check[25]); // the large node button is highlighted
                        }
                    }
                }
            }
        }
        else
        {
            if ( location.y < 330 )
            {
                if (location.x < 460)
                {
                    if (!check[26])
                    {
                        for (int i=22; i<=36; i++)
                        {
                            check[i] = false;
                        }
                        menuFlip(optionsSmallLineHover.surface(), check[26]); // the small line button is highlighted
                    }
                }
                else
                {
                    if (location.x < 600)
                    {
                        if (!check[27])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            menuFlip(optionsMediumLineHover.surface(), check[27]); // the medium line button is highlighted
                        }
                    }
                    else
                    {
                        if (location.x < 700)
                        {
                            if (!check[28])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                menuFlip(optionsLargeLineHover.surface(), check[28]); // the large line button is highlighted
                            }
                        }
                    }
                }
            }
            else
            {
                if ( location.y < 400 )
                {
                    if (location.x < 395)
                    {
                        if (!check[29])
                        {
                            for (int i=22; i<=36; i++)
                            {
                                check[i] = false;
                            }
                            menuFlip(optionsThreeNodesHover.surface(), check[29]); // the three nodes button is highlighted
                        }
                    }
                    else
                    {
                        if (location.x < 440)
                        {
                            if (!check[30])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                menuFlip(optionsFourNodesHover.surface(), check[30]); // the four nodes button is highlighted
                            }
                        }
                        else
                        {
                            if (location.x < 485)
                            {
                                if (!check[31])
                                {
                                    for (int i=22; i<=36; i++)
                                    {
                                        check[i] = false;
                                    }
                                    menuFlip(optionsFiveNodesHover.surface(), check[31]); // the five nodes button is highlighted
                                }
                            }
                            else
                            {
                                if (location.x < 520)
                                {
                                    if (!check[32])
                                    {
                                        for (int i=22; i<=36; i++)
                                        {
                                            check[i] = false;
                                        }
                                        menuFlip(optionsSixNodesHover.surface(), check[32]); // the six nodes button is highlighted
                                    }
                                }
                                else
                                {
                                    if (location.x < 565)
                                    {
                                        if (!check[33])
                                        {
                                            for (int i=22; i<=36; i++)
                                            {
                                                check[i] = false;
                                            }
                                            menuFlip(optionsSevenNodesHover.surface(), check[33]); // the seven nodes button is highlighted
                                        }
                                    }
                                    else
                                    {
                                        if (!check[22])
                                        {
                                            for (int i=22; i<=36; i++)
                                            {
                                                check[i] = false;
                                            }
                                            menuFlip(options.surface(), check[22]); // no buttons are highlighted if the mouse is too far right of the seven nodes button
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (location.y < 450)
                    {
                        if (location.x < 540)
                        {
                            if (!check[34])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                menuFlip(optionsPvPHover.surface(), check[34]); // the two player game mode button is highlighted
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
                                menuFlip(optionsPvAIHover.surface(), check[35]); // the one player game mode button is highlighted
                            }
                        }
                    }
                    else
                    {
                        if (location.y < 510)
                        {
                            if (!check[22])
                            {
                                for (int i=22; i<=36; i++)
                                {
                                    check[i] = false;
                                }
                                    menuFlip(options.surface(), check[22]); // no buttons are highlighted between the game mode option and the back button
                            }
                        }
                        else
                        {
                            if (location.x < 450)
                            {
                                if (!check[36])
                                {
                                    for (int i=22; i<=36; i++)
                                    {
                                        check[i] = false;
                                    }
                                    menuFlip(optionsBackHover.surface(), check[36]); // the back button is highlighted
                                }
                            }
                            else
                            {
                                if (!check[22])
                                {
                                    for (int i=22; i<=36; i++)
                                    {
                                        check[i] = false;
                                    }
                                        menuFlip(options.surface(), check[22]); // no buttons are highlighted below the back button
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

ClickType Menu::click(Coord location) // tells the program which buttons to activate. the previous three functions basically show the buttons and the following three allow them to be clicked on
{
    if (GameOver)
        return clickGameOver(location);
    else
        return clickMenu(location);
}

ClickType Menu::clickMenu(Coord location)
{
    if ((GameOptions == false) && (GameInstructions == false) && (GameCredits == false)) // while looking at the main menu screen
    {
        if (location.y > 250)
        {
            if (location.y < 300) // the start button is clicked
            {
                GameStart = true;
                return GAME;
            }
            else
            {
                if (location.y < 350) // the options button is clicked
                {
                    GameOptions = true;
                    menuFlip(options.surface(), GameOptions);
                }
                else
                {
                    if (location.y < 400) // the instructions button is clicked
                    {
                        GameInstructions = true;
                        menuFlip(instructions.surface(), check[9]);
                        check[10] = false;
                    }
                    else
                    {
                        if (location.y < 450) // the credits button is clicked
                        {
                            GameCredits = true;
                            menuFlip(credits.surface(), check[11]);
                            check[12] = false;
                        }
                        else
                        {
                            if (location.y < 500) // the exit button is clicked
                            {
                                // Return EXIT to end the game
                                return EXIT; // returning this to the main ends the game and closes the window
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (GameOptions == true) // there is a separate function for all of the options buttons
        {
            optionsPageClick(location);
        }
        else // back buttons for the instructions and credits pages are in the same place so the following code is used for both
        {
            if (location.y > 550)
            {
                GameInstructions = GameCredits = false;
                menuFlip(sproutsMenu.surface(), check[1]);
            }
        }
    }
    return CONTINUE; // if no button is pressed then returning this to the main tells the program to loop around again
}

ClickType Menu::clickGameOver(Coord location) // the game over menu
{
    if ((location.y > 390) && (location.y <490))
    {
        if (location.y < 440) // returns the player to the main menu
        {
            GameOver = p1Wins = p2Wins = aiWins = false;
            init();
        }
        else // exits out of the game without having to go throught the main
        {
            // Return EXIT to end the game
            return EXIT; // returning this to the main ends the game and closes the window
        }
    }
    return CONTINUE; // if no button is pressed then returning this to the main tells the program to loop around again
}

void Menu::optionsPageClick(Coord location) // the following function changes changes the conditions of the game based on where the player clicks
{
    if ((location.y > 200) && (location.y < 575) && (location.x > 345) && (location.x < 700))
    {
        if ( location.y < 260 )
        {
            if (location.x < 460) // the small node radius button is pressed
            {
                optionsNodeRadius = 5;
            }
            else
            {
                if (location.x < 600) // the medium node radius button is pressed
                {
                    optionsNodeRadius = 10;
                }
                else
                {
                    if (location.x < 700) // the large node radius button is pressed
                    {
                        optionsNodeRadius = 15;
                    }
                }
            }
        }
        else
        {
            if ( location.y < 330 )
            {
                if (location.x < 460) // the small line thickness button is pressed
                {
                    optionsLineThick = 1;
                }
                else
                {
                    if (location.x < 600) // the medium line thickness button is pressed
                    {
                        optionsLineThick = 2;
                    }
                    else
                    {
                        if (location.x < 700) // the large line thickness button is pressed
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
                    if (location.x < 395) // the three nodes button is pressed
                    {
                        numberOfNodes = 3;
                    }
                    else
                    {
                        if (location.x < 440) // the four nodes button is pressed
                        {
                            numberOfNodes = 4;
                        }
                        else
                        {
                            if (location.x < 485) // the five nodes button is pressed
                            {
                                numberOfNodes = 5;
                            }
                            else
                            {
                                if (location.x < 520) // the six nodes button is pressed
                                {
                                    numberOfNodes = 6;
                                }
                                else
                                {
                                    if (location.x < 565) // the seven nodes button is pressed
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
                        if (location.x > 540) // the two player button is pressed
                        {
                            playerMode = PvP;
                        }
                        else // the one player button is pressed
                        {
                            playerMode = PvAI;
                        }
                    }
                    else
                    {
                        if ((location.y > 510) && (location.x < 450)) // the back button is pressed
                        {
                            GameOptions = false;
                            menuFlip(sproutsMenu.surface(), check[1]);
                        }
                    }
                }
            }
        }
    }
}

void Menu::cancel()
{

}

Menu::~Menu()
{

}
