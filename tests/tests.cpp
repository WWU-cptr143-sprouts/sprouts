#include "tests.h"

void test(const string& s, bool passed)
{
    cout << s << ": " << ((passed)?"passed":"failed") << endl;
}
