#ifndef H_GameGUI
#define H_GameGUI

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "game.h"

using namespace std;

class GameGUI : public Game
{
    public:
        GameGUI(SDL_Surface*,SDL_Surface*,int); // default constructor : feed it a surface, buffer, and initial sprouts number

        ~GameGUI();
};

#endif
