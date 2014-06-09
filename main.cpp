#include "platform.h"
#include "menu.h"
/*
This is an extremely simple function, and yet it is where everything actually runs.
All main does is call startGraphics to set up things, mainMenu to set up the main menu,
and when the main menu return endGraphics is called to clean up everything
*/
int main(int argc, char ** argv)
{
    //TODO: implement command line argument parsing
    startGraphics();
    mainMenu();
    endGraphics();
    return 0;
}
