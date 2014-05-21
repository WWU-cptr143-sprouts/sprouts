#include "platform.h"
#include "menu.h"

int main(int argc, char ** argv)
{
    //TODO: implement command line argument parsing
    startGraphics();
    mainMenu();
    endGraphics();
    return 0;
}
