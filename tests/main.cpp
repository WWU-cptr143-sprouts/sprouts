#include "tests.h"

int main()
{
    Tests().game1();
    Tests().game2();
    Tests().game1_doMove();
    Tests().invalidCorner();
    Tests().invalidLine();
    Tests().imageNotLoaded();
    copyConstructor();

    return 0;
}
