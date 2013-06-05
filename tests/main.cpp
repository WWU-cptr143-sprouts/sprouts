#include "tests.h"

int main()
{
    char dummy;
    eqtester();
    cout << "pause" << endl;
    cin >> dummy;
    Tests().game1();
    Tests().game2();
    Tests().game1_doMove();
    Tests().invalidCorner();
    Tests().invalidLine();
    Tests().imageNotLoaded();
    copyConstructor();

    return 0;
}
