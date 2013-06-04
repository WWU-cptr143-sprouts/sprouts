#include "tests.h"

void Tests::sdlRenderGame() const
{
    int width = 800;
    int height = 602;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_LockSurface(screen);
    SDL_WM_SetCaption("SDL Render Game - Test Suite", "SDL Render Game - Test Suite");

    static const int scale = 5; // Otherwise it's always in the top left corner
    static const int radius = 5;
    static const Uint32 nodeCol = 0xFF4500FF;
    static const Uint32 lineCol = 0xFFFFFFFF;

    for (int i = 0; i < nodes.size(); i++)
        circleColor(screen, nodes[i]->getLoci().x*scale,
                            nodes[i]->getLoci().y*scale, radius, nodeCol);

    for (int i = 0; i < lines.size(); i++)
        for (int j = 1; j < lines[i]->size(); j++)
            lineColor(screen, (*lines[i])[j-1].x*scale, (*lines[i])[j-1].y*scale,
                              (*lines[i])[j].x*scale,   (*lines[i])[j].y*scale,
                              lineCol);

    SDL_UnlockSurface(screen);
    SDL_Flip(screen);

    cout << "Press enter to continue." << endl;
    string s;
    getline(cin,s);

    SDL_FreeSurface(screen);
    SDL_Quit();
}
