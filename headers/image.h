#ifndef H_IMAGE
#define H_IMAGE

#include <SDL/SDL.h>
#include <string>

using namespace std;

class Image
{
    private:
        SDL_Surface *img;

    public:
        Image(const string&);
        SDL_Surface* surface();
        ~Image();
};

#endif
