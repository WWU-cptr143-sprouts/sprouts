#ifndef H_MENU
#define H_MENU

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <string>
#include <algorithm>
#include "image.h"
#include "structs.h"

enum Mode
{
    PvP,
    PvAI
};

enum ClickType
{
    CONTINUE, GAME, EXIT
};

class Menu
{
    private:
        SDL_Surface* screen;

        int optionsNodeRadius; // Node radius
        int optionsLineThick; // Line thickness
        int optionsSelectRadius; // Defines how close you must click to a node

        Mode playerMode;
        int numberOfNodes;
        Image instructions;
        Image instructionsBackHover;
        Image credits;
        Image creditsBackHover;
        Image exitHover;
        Image creditsHover;
        Image instructionsHover;
        Image optionsHover;
        Image startHover;
        Image sproutsMenu;

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

        Image computerWins;
        Image computerWinsExitHover;
        Image computerWinsMenuHover;
        Image playerOneWins;
        Image playerOneWinsExitHover;
        Image playerOneWinsMenuHover;
        Image playerTwoWins;
        Image playerTwoWinsExitHover;
        Image playerTwoWinsMenuHover;

        bool GameStart;
        bool GameOptions;
        bool GameInstructions;
        bool GameCredits;
        bool GameExit;
        bool GameOver;
        bool p1Wins;
        bool p2Wins;
        bool aiWins;

        bool check[37];
        bool check1, check2,  check3,  check4,
             check5, check6,  check7,  check8,
             check9, check10, check11, check12,
             check13, check14, check15, check16,
             check17, check18, check19, check20,
             check21;

    public:
        Menu(SDL_Surface*);

        void menuFlip(SDL_Surface*, bool&);
        void init(); // Show the menu the first time
        void over(bool); // Tell menu that the game is over
        void cancel(); // Esc pressed
        void cursor(Coord); // Cursor moved
        void cursorGameOver(Coord);
        void cursorMenu(Coord);
        ClickType click(Coord); // Return EXIT when clicking exit
        ClickType clickGameOver(Coord);
        ClickType clickMenu(Coord);
        void optionsPageCursor(Coord);
        void optionsPageClick(Coord);

        int nodes() const { return numberOfNodes; }
        Mode mode() const { return playerMode; }
        int nodeRadius() const { return optionsNodeRadius; }
        int selectRadius() const { return optionsSelectRadius; }
        int lineThick() const { return optionsLineThick; }

        ~Menu();
};

#endif
