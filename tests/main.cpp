#include "tests.h"

int main()
{
    Tests().game1();
    Tests().game1_doMove();
    Tests().invalidCorner();
    Tests().invalidLine();
    Tests().imageNotLoaded();

    return 0;
}
