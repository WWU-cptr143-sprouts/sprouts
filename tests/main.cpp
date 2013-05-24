#include <string>
#include <iostream>
#include "../headers/game.h"

using namespace std;

void test(const string& s, bool passed)
{
    cout << s << ": " << ((passed)?"passed":"failed") << endl;
}

class TestSuite : public Game
{
    public:
        TestSuite();
};

TestSuite::TestSuite()
{
    Game g;
    bool passed = false;

    g.updateAreas();

    Node a(Coord(0,0));
    Node b(Coord(10,10));
    Node c(Coord(20,20));

    g.insertNode(a);
    g.insertNode(b);
    g.insertNode(c);

    g.updateAreas();

    // Test 1, all original nodes connectable
    passed = true;

    if (!g.connectable(a, b) ||
        !g.connectable(a, c) ||
        !g.connectable(b, a) ||
        !g.connectable(b, c) ||
        !g.connectable(c, a) ||
        !g.connectable(c, b))
        passed = false;

    test("1", passed);

    // Draw a line down 10 pixels and over 10 pixels from A to B
    /*Line l;
    l.push_back(Coord(0,0));
    l.push_back(Coord(0,10));
    l.push_back(Coord(10,10));*/

    // Place a coordinate at (5,10), somehow split line into A->D and D->B
    Line AD;
    AD.push_back(Coord(0,0));
    AD.push_back(Coord(0,10));
    AD.push_back(Coord(5,10));
    Line DB;
    DB.push_back(Coord(5,10));
    DB.push_back(Coord(10,10));

    // Check if A and B can be connected...
    //if (g.connectable(a, b))

    // If so, save and get the unique identifier (the pointer)
    lines.push_back(AD);
    Line* AD_ptr = &lines.back();
    lines.push_back(DB);
    Line* DB_ptr = &lines.back();

    // Put the node on second line
    Node d(Coord(5, 10),
        Connection(AD_ptr, &a),  // From A to D
        Connection(DB_ptr, &b)); // From D to B

    // Add connection to A
    a.addConnection(Connection(AD_ptr, &d));

    // Test 2, all 2nd play nodes connectable
    passed = true;

    if (!g.connectable(a, b) ||
        !g.connectable(a, c) ||
        !g.connectable(a, d) ||
        !g.connectable(b, a) ||
        !g.connectable(b, c) ||
        !g.connectable(b, d) ||
        !g.connectable(c, a) ||
        !g.connectable(c, b) ||
        !g.connectable(c, d) ||
        !g.connectable(d, a) ||
        !g.connectable(d, b) ||
        !g.connectable(d, c))
        passed = false;

    test("2", passed);

}

int main()
{
    TestSuite t;
    return 0;
}
