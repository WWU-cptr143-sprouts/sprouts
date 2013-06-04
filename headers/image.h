#ifndef H_IMAGE
#define H_IMAGE

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include <string>

using namespace std;

class ImageNotLoaded { };

class Image
{
    private:
        SDL_Surface *img;

    public:
        Image(const string&, bool transparent = false);
        SDL_Surface* surface();
        ~Image();
};

#endif
