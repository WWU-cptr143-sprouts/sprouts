#ifndef H_BUTTON
#define H_BUTTON

#include <string>
#include "draw.h"
#include "../stdafx.h"
#include <SDL/SDL_ttf.h>


// Base Class for buttons, which should include all basic button functionality
// which can be easily derived from for specialty buttons
enum ButtonState
{
    B_STATE_NORMAL,
    B_STATE_HOVER,
    B_STATE_PRESSED
};

class Button
{
    TTF_Font * font;
    ButtonState state;

    // (x,y) location and (w,h) of the button
    Point point;

    // Where the image is that will need to be clipped
    SDL_Surface * text;

public:
    Button(Point placement, std::string textString, TTF_Font * theFont);

    ButtonState getState();
    bool setState(ButtonState newState);
    bool isMouseover(int x, int y);

    // Draws the button based on the state given and the (x,y) value given
    bool draw(SDL_Surface* destination);

    // Moves the button to the requested (x,y) coordinates
    bool moveTo(int x, int y);

    ~Button();
};

#endif
