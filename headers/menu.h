#ifndef H_MENU
#define H_MENU

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <string>
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
        string player1Name;
        string player2Name;
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

        int nodes() const { return numberOfNodes; }
        Mode mode() const { return playerMode; }
        const string& player1() const { return player1Name; }
        const string& player2() const { return player2Name; }

        ~Menu();
};

#endif
