#ifndef H_IMAGE
#define H_IMAGE

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include <string>
#include <iostream>

using namespace std;

// Thrown when the image could not be loaded
class ImageNotLoaded
{
    string s;

    public:
        ImageNotLoaded(const string& s) :s(s) { }
        friend ostream& operator<<(ostream&, const ImageNotLoaded&);
};

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
