#include "tests.h"

int main()
{
    Tests().game1();
    Tests().game2();
    Tests().jaggedLine();
    Tests().game1_doMove();
    Tests().invalidCorner();
    Tests().invalidLine();
    Tests().imageNotLoaded();
    copyConstructor();
    eqtester();

    return 0;
}
