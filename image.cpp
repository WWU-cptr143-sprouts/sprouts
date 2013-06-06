#include "headers/image.h"

Image::Image(const string& s, bool transparent)
{
    SDL_Surface* tmpImg = IMG_Load(s.c_str());

    if (!tmpImg)
        throw ImageNotLoaded();

    // Optimize image for depth of screen
    img = SDL_DisplayFormat(tmpImg);
    SDL_FreeSurface(tmpImg);

    if (!img)
        throw ImageNotLoaded();

    // Enable transparency
    if (transparent)
    {
        Uint32 colorkey = SDL_MapRGB(img->format, 255, 255, 255);
        SDL_SetColorKey(img, SDL_SRCCOLORKEY, colorkey);
    }
}

SDL_Surface* Image::surface()
{
    return img;
}

Image::~Image()
{
    SDL_FreeSurface(img);
}
