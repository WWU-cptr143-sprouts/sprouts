#ifndef H_DRAW
#define H_DRAW

#include <SDL.h>

/* Source : http://sdl.beuc.net/sdl.wiki/Introduction_to_SDL_Video
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 * NOTE 2: SDL_gfx has point drawing functionality - this one is for my experience
 */

Uint32 getpixel(SDL_Surface *surface, int x, int y);

#endif
