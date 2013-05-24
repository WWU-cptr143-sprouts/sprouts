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

// This is inherited from Game because this is testing it as if it were
// GameGUI, using user input.
TestSuite::TestSuite()
    :Game()
{
    bool passed = false;

    updateAreas();

    Node& a = insert(Node(Coord(0,0)));
    Node& b = insert(Node(Coord(10,10)));
    Node& c = insert(Node(Coord(20,20)));

    updateAreas();

    // Test 1, all original nodes connectable
    passed = true;

    if (!connectable(a, b) ||
        !connectable(a, c) ||
        !connectable(b, a) ||
        !connectable(b, c) ||
        !connectable(c, a) ||
        !connectable(c, b))
        passed = false;

    test("1", passed);

    // Draw a line down 10 pixels and over 10 pixels from A to B
    /*Line l;
    l.push_back(Coord(0,0));
    l.push_back(Coord(0,10));
    l.push_back(Coord(10,10));*/

    // Place a coordinate at (5,10), somehow split line into A->D and D->B
    Line AD_line;
    AD_line.push_back(Coord(0,0));
    AD_line.push_back(Coord(0,10));
    AD_line.push_back(Coord(5,10));
    Line DB_line;
    DB_line.push_back(Coord(5,10));
    DB_line.push_back(Coord(10,10));

    // Check if A and B can be connected...
    //if (g.connectable(a, b))

    // If so, save and get the unique identifier (the pointer)
    Line& AD = insert(AD_line);
    Line& DB = insert(DB_line);

    // Put the node on second line
    Node& d = insert(Node(Coord(5, 10),
        Connection(&AD, &a),  // From A to D
        Connection(&DB, &b))); // From D to B

    // Add connection from A and from B
    a.addConnection(Connection(&AD, &d));
    b.addConnection(Connection(&DB, &d));

    // Test 2, all 2nd play nodes connectable
    passed = true;

    if (!connectable(a, b) ||
        !connectable(a, c) ||
        !connectable(a, d) ||
        !connectable(b, a) ||
        !connectable(b, c) ||
        !connectable(b, d) ||
        !connectable(c, a) ||
        !connectable(c, b) ||
        !connectable(c, d) ||
        !connectable(d, a) ||
        !connectable(d, b) ||
        !connectable(d, c))
        passed = false;

    test("2", passed);

    // Draw line around C, from D to B
    /*Line l;
    l.push_back(Coord(5,10));
    l.push_back(Coord(5,30));
    l.push_back(Coord(30,30));
    l.push_back(Coord(30,5));
    l.push_back(Coord(10,5));
    l.push_back(Coord(10,10));*/
    
    // Place a coordinate at (10,30), somehow split line into D->E and E->B
    Line DE_line;
    DE_line.push_back(Coord(5,10));
    DE_line.push_back(Coord(5,30));
    DE_line.push_back(Coord(10,30));
    Line EB_line;
    EB_line.push_back(Coord(10,30));
    EB_line.push_back(Coord(30,30));
    EB_line.push_back(Coord(30,5));
    EB_line.push_back(Coord(10,5));
    EB_line.push_back(Coord(10,10));

    // Check if D and B can be connected...
    //if (g.connectable(d, b))

    // If so, save and get the unique identifier (the pointer)
    Line& DE = insert(DE_line);
    Line& EB = insert(EB_line);

    // Put the node on second line
    Node& e = insert(Node(Coord(10, 30),
        Connection(&DE, &d),  // From D to E
        Connection(&EB, &b))); // From E to B

    // Add connection to D
    d.addConnection(Connection(&DE, &e));
    b.addConnection(Connection(&EB, &e));
}

int main()
{
    TestSuite t;
    return 0;
}
