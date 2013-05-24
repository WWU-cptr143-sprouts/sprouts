#ifndef H_GameGUI
#define H_GameGUI

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "game.h"

using namespace std;

class GameGUI : public Game
{
    private:
        SDL_Surface* screen;
    public:
        GameGUI(SDL_Surface*,int); // default constructor : feed it a surface for the screen
                                   // and initial sprouts number

        void cancel(); // Escape pressed
        void click(int x, int y); // Mouse clicked at (x,y)
        void cursor(int x, int y); // Cursor hovering at (x,y)

        // Find coordinate of center of screen
        Coord center() const;

        ~GameGUI();
};

#endif
