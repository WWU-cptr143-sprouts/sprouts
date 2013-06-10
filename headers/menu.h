#ifndef H_MENU
#define H_MENU

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <string>
#include <algorithm>
#include "image.h"
#include "structs.h"

enum Mode // values attached to the playerMode variable. They represent "player vs player" and "player vs AI"
{
    PvP,
    PvAI
};

enum ClickType // values return from the click functions. They are used to determine what to do next.
{
    CONTINUE, GAME, EXIT
};

class Menu
{
    private:
        SDL_Surface* screen; // The background

        int optionsNodeRadius; // Node radius
        int optionsLineThick; // Line thickness
        int optionsSelectRadius; // Defines how close you must click to a node

        Mode playerMode; // One player or two
        int numberOfNodes; // can be anything but the menu only allows for 3 to 7
        // All of the different pages and buttons in the menu are different images blitted onto the background
        // The following variables are loaded with the different images in the menu.cpp file
        Image sproutsMenu;
        Image startHover;
        Image optionsHover;
        Image instructionsHover;
        Image creditsHover;
        Image exitHover;

        Image options;
        Image optionsSmallNodeHover;
        Image optionsMediumNodeHover;
        Image optionsLargeNodeHover;
        Image optionsSmallLineHover;
        Image optionsMediumLineHover;
        Image optionsLargeLineHover;
        Image optionsThreeNodesHover;
        Image optionsFourNodesHover;
        Image optionsFiveNodesHover;
        Image optionsSixNodesHover;
        Image optionsSevenNodesHover;
        Image optionsPvPHover;
        Image optionsPvAIHover;
        Image optionsBackHover;

        Image instructions;
        Image instructionsBackHover;

        Image credits;
        Image creditsBackHover;

        Image computerWins;
        Image computerWinsExitHover;
        Image computerWinsMenuHover;
        Image playerOneWins;
        Image playerOneWinsExitHover;
        Image playerOneWinsMenuHover;
        Image playerTwoWins;
        Image playerTwoWinsExitHover;
        Image playerTwoWinsMenuHover;

        // the following variables are used to direct the menu and tell which page to show and which buttons to use
        bool GameStart;
        bool GameOptions;
        bool GameInstructions;
        bool GameCredits;
        bool GameExit;
        bool GameOver;
        bool p1Wins;
        bool p2Wins;
        bool aiWins;

        // the following variables are used to prevent the code from attempting to upload the same background twice in a row
        bool check[37];

    public:
        // the constructor function
        Menu(SDL_Surface*);

        // the nonconstructor functions
        void menuFlip(SDL_Surface*, bool&); // This function makes the background in the menu look like the contents of one of the Image clss variables
        void init(); // Show the menu the first time
        void over(bool); // Tell menu that the game is over
        void cancel(); // Esc pressed
        void cursor(Coord); // Cursor moved
        void cursorGameOver(Coord);
        void cursorMenu(Coord);
        ClickType click(Coord); // Return EXIT when clicking exit
        ClickType clickGameOver(Coord);
        ClickType clickMenu(Coord);
        void optionsPageCursor(Coord); // A separate page for all of the possible options
        void optionsPageClick(Coord);

        // the following functions return the values of the following five variables from the menu class
        int nodes() const { return numberOfNodes; }
        Mode mode() const { return playerMode; }
        int nodeRadius() const { return optionsNodeRadius; }
        int selectRadius() const { return optionsSelectRadius; }
        int lineThick() const { return optionsLineThick; }

        ~Menu();
};

#endif
