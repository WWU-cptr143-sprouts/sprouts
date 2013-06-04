#include "tests.h"

void Tests::game1()
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

    test("game1 1", passed);

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

    test("game1 2", passed);

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

    //Tests InvalidCorner()
    //Line EB_line;
    //EB_line.push_back(Coord(10,30));
    //EB_line.push_back(Coord(30,30));
    //EB_line.push_back(Coord(30,5));
    //EB_line.push_back(Coord(10,5));
    //EB_line.push_back(Coord(10,10));

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

    test("game1 3", passed);

    // Test 4, another point outside all areas
    Node& f = insertNode(Coord(50,50));

    updateAreas();
    passed = false;

    if (connectable(a,f) &&
        connectable(b,f) &&
        connectable(e,f) &&
        connectable(f,a) &&
        connectable(f,b) &&
        connectable(f,e) &&
        !connectable(d,f) &&
        !connectable(c,f) &&
        !connectable(f,d) &&
        !connectable(f,c))
        passed = true;

    test("game1 4", passed);
}

void Tests::game1_doMove()
{
    bool passed = false;

    Node& a = insertNode(Coord(0,0));
    Node& b = insertNode(Coord(10,10));
    Node& c = insertNode(Coord(20,20));
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

    test("game1_doMove 1", passed);

    // Draw a line down 10 pixels and over 10 pixels from A to B
    Line l;
    l.push_back(Coord(0,0));
    l.push_back(Coord(0,10));
    l.push_back(Coord(10,10));

    doMove(l, Coord(5, 10));
    Node& d = *nodes.back();

    // Test 2
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

    test("game1_doMove 2", passed);

    // Draw line around C, from D to B
    l.clear();
    l.push_back(Coord(5,10));
    l.push_back(Coord(5,30));
    l.push_back(Coord(30,30));
    l.push_back(Coord(30,10));
    //l.push_back(Coord(30,5));
    //l.push_back(Coord(10,5));
    l.push_back(Coord(10,10));

    doMove(l, Coord(10, 30));
    Node& e = *nodes.back();

    // Test 3, some non-connectable nodes
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

    test("game1_doMove 3", passed);
    //sdlRenderGame();
}

void Tests::copyInitialize()
{
    Node& a = insertNode(Coord(0,0));
    Node& b = insertNode(Coord(10,10));
    Node& c = insertNode(Coord(20,20));
    updateAreas();

    Line l;
    l.push_back(Coord(0,0));
    l.push_back(Coord(0,10));
    l.push_back(Coord(10,10));

    doMove(l, Coord(5, 10));
    Node& d = *nodes.back();

    l.clear();
    l.push_back(Coord(5,10));
    l.push_back(Coord(5,30));
    l.push_back(Coord(30,30));
    l.push_back(Coord(30,10));
    l.push_back(Coord(10,10));

    doMove(l, Coord(10, 30));
    Node& e = *nodes.back();
}

bool Tests::validateCopy() const
{
    Node& a = *nodes[0];
    Node& b = *nodes[1];
    Node& c = *nodes[2];
    Node& d = *nodes[3];
    Node& e = *nodes[4];

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
        return true;

    return false;
}

void copyConstructor()
{
    Tests t;
    t.copyInitialize();
    test("copyConstructor 1", t.validateCopy());

    Tests t2(t);
    t2.updateAreas(); // Must update since copying ignores the areas (no point
                      // in updating when you'll change something anyway)
    test("copyConstructor 2", t2.validateCopy());
}
