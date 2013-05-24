#include "headers/image.h"

Image::Image(const string& s)
{
    img = SDL_LoadBMP(s.c_str());

    if (!img)
        throw ImageNotLoaded();

    Uint32 colorkey = SDL_MapRGB(img->format, 255, 255, 255);
    SDL_SetColorKey(img, SDL_SRCCOLORKEY, colorkey);
}

SDL_Surface* Image::surface()
{
    return img;
}

Image::~Image()
{
    SDL_FreeSurface(img);
}
