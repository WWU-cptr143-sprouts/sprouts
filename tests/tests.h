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
        void invalidCorner();
        void invalidLine();
        void imageNotLoaded();
};

// Print out test results
void test(const string& s, bool passed);

#endif
