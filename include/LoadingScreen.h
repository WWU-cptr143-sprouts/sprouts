#ifndef H_LOADING
#define H_LOADING

#include "draw.h"
#include <string>

class LoadingScreen
{
    int frame;
    int ticksStart;
    std::string folder;
    std::string screenname;
    std::string waitname;
    bool isWait, isStart;
    SDL_Surface *loadingScreen[150];
    SDL_Surface *loadingWait;
    SDL_Surface *loadingLoad[150];
    bool initing;

public:
    LoadingScreen();

    // Music
    void PlayWait();
    void PlayStart();

    // Graphics
    bool draw(SDL_Surface* destination);
};

#endif
