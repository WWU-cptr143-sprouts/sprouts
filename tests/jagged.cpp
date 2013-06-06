#include "tests.h"

void Tests::jaggedLine()
{
    insertNode(Coord(10,30));
    insertNode(Coord(40,10));
    updateAreas();

    Line l;
    l.push_back(Coord(10,30));
    l.push_back(Coord(20,30));
    l.push_back(Coord(20,20));
    l.push_back(Coord(30,20));
    l.push_back(Coord(30,40));
    l.push_back(Coord(40,40));
    l.push_back(Coord(40,10));

    bool passed = true;

    try
    {
        doMove(l, Coord(25, 20));
    }
    catch (const InvalidMiddle& e)
    {
        passed = false;
        cout << e << endl;
    }
    catch (...)
    {
        passed = false;
    }

    test("jaggedLine", passed);
}
