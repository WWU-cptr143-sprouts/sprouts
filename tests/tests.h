#ifndef H_TESTS
#define H_TESTS

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include "../headers/game.h"
#include "../headers/image.h"

using namespace std;

class Tests : public Game
{
    public:
        // This is inherited from Game because this is testing it as if it were
        // GameGUI, using user input.
        Tests() :Game() { }

        void game1();
        void game2();
        void game1_doMove();
        void invalidCorner();
        void invalidLine();
        void imageNotLoaded();

        // Initialize game for the copyConstructor test
        void copyInitialize();
        // See if the copy was successful (needs to access protected variables)
        bool validateCopy() const;

        // Show the game in a GUI for easier debugging
        void sdlRenderGame() const;
};

// Test copying the game and see if it's still valid like it should be.
void copyConstructor();

// Print out test results
void test(const string& s, bool passed);

#endif
