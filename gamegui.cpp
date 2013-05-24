#include "headers/gamegui.h"

GameGUI::GameGUI(SDL_Surface* screen, int count)
    :screen(screen)
{
    // Create count nodes
}

void GameGUI::cancel()
{

}

void GameGUI::click(int x, int y)
{

}

void GameGUI::cursor(int x, int y)
{

}

Coord GameGUI::center() const
{
    return Coord(screen->w/2, screen->h/2);
}

GameGUI::~GameGUI()
{

}
