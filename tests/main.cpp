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
    updateAreas();
    bool passed = false;

    Node& a = insertNode(Coord(0,0));
    Node& b = insertNode(Coord(10,10));
    Node& c = insertNode(Coord(20,20));

    // Test 1, all original nodes connectable
    updateAreas();
    passed = true;

    if (!connectable(a, b) ||
        !connectable(a, c) ||
        !connectable(b, a) ||
        !connectable(b, c) ||
        !connectable(c, a) ||
        !connectable(c, b))
        passed = false;

    test("1", passed);
    cout << *this;

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
    Line& AD = insertLine(AD_line);
    Line& DB = insertLine(DB_line);

    // Put the node on second line
    Node& d = insertNode(Coord(5, 10),
        Connection(&AD, &a),  // From A to D
        Connection(&DB, &b)); // From D to B

    // Add connection from A and from B
    a.addConnection(Connection(&AD, &d));
    b.addConnection(Connection(&DB, &d));

    // Test 2, all 2nd play nodes connectable
    updateAreas();
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
    cout << *this;

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
    EB_line.push_back(Coord(30,10));
    EB_line.push_back(Coord(10,10));

    // Check if D and B can be connected...
    //if (g.connectable(d, b))

    // If so, save and get the unique identifier (the pointer)
    Line& DE = insertLine(DE_line);
    Line& EB = insertLine(EB_line);

    // Put the node on second line
    Node& e = insertNode(Coord(10, 30),
        Connection(&DE, &d),  // From D to E
        Connection(&EB, &b)); // From E to B

    // Add connection to D
    d.addConnection(Connection(&DE, &e));
    b.addConnection(Connection(&EB, &e));

    // Test 3, some non-connectable nodes
    updateAreas();
    passed = false;

    if (connectable(a,b) &&
        connectable(a,e) &&
        connectable(b,a) &&
        connectable(b,c) &&
        connectable(b,e) &&
        connectable(c,b) &&
        connectable(c,e) &&
        connectable(e,a) &&
        connectable(e,b) &&
        connectable(e,c) &&
        !connectable(a,c) &&
        !connectable(a,d) &&
        !connectable(b,d) &&
        !connectable(c,a) &&
        !connectable(c,d) &&
        !connectable(d,a) &&
        !connectable(d,b) &&
        !connectable(d,c) &&
        !connectable(d,e) &&
        !connectable(e,d))
        passed = true;

    test("3", passed);
    cout << *this;

    cout << "AB is " << ((connectable(a,b))?"":"not ") << "connectable." << endl;
    cout << "AE is " << ((connectable(a,e))?"":"not ") << "connectable." << endl;
    cout << "BA is " << ((connectable(b,a))?"":"not ") << "connectable." << endl;
    cout << "BC is " << ((connectable(b,c))?"":"not ") << "connectable." << endl;
    cout << "BE is " << ((connectable(b,e))?"":"not ") << "connectable." << endl;
    cout << "CB is " << ((connectable(c,b))?"":"not ") << "connectable." << endl;
    cout << "CE is " << ((connectable(c,e))?"":"not ") << "connectable." << endl;
    cout << "EA is " << ((connectable(e,a))?"":"not ") << "connectable." << endl;
    cout << "EB is " << ((connectable(e,b))?"":"not ") << "connectable." << endl;
    cout << "EC is " << ((connectable(e,c))?"":"not ") << "connectable." << endl;
    cout << "AC is " << ((connectable(a,c))?"":"not ") << "connectable." << endl;
    cout << "AD is " << ((connectable(a,d))?"":"not ") << "connectable." << endl;
    cout << "BD is " << ((connectable(b,d))?"":"not ") << "connectable." << endl;
    cout << "CA is " << ((connectable(c,a))?"":"not ") << "connectable." << endl;
    cout << "CD is " << ((connectable(c,d))?"":"not ") << "connectable." << endl;
    cout << "DA is " << ((connectable(d,a))?"":"not ") << "connectable." << endl;
    cout << "DB is " << ((connectable(d,b))?"":"not ") << "connectable." << endl;
    cout << "DC is " << ((connectable(d,c))?"":"not ") << "connectable." << endl;
    cout << "DE is " << ((connectable(d,e))?"":"not ") << "connectable." << endl;
    cout << "ED is " << ((connectable(e,d))?"":"not ") << "connectable." << endl;
}

int main()
{
    TestSuite();
    return 0;
}
