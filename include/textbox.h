#ifndef H_TEXTBOX
#define H_TEXTBOX

#include "../stdafx.h"

#include <string>
#include <SDL/SDL_ttf.h>
#include "draw.h"
#include "game.h"

enum TextboxFocus
{
    TEXTBOX_NO_FOCUS,
    TEXTBOX_FOCUS
};

enum TextboxType
{
    TEXTBOX_TYPE_NUM,
    TEXTBOX_TYPE_MOVE
};

class TextBox {
    Game * game;
    SDL_Color textColor;
    TTF_Font * font;
    SDL_Surface * textRender;
    SDL_Surface * destination;
    int ticksStart;
    Point point;
    bool changed;        // Whether we should re render the string or not
    TextboxFocus state;  // Holds whether it has focus or not
    TextboxType type;    // Holds what type of box it is so that we know where
                         // to send the string on enter)
    int cursor;          // Where in the animation the cursor is
    std::string textString;

public:
    TextBox(TTF_Font * theFont, SDL_Color color, TextboxType type,
        Point placement, Game * theGame);

    bool draw(SDL_Surface * destination);
    bool processKeyDown(SDL_Event * event);
    bool setState(TextboxFocus state);
    TextboxFocus getState(void);
    
    // Determine the space where mouse input can be accepted
    bool isMouseover(int x, int y);

    ~TextBox();
};

#endif
