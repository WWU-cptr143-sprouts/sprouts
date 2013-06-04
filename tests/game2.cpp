#include "tests.h"

void Tests::game2()
{
    bool passed = false;

    Node& a = insertNode(Coord(10,60));
    Node& b = insertNode(Coord(20,30));
    Node& c = insertNode(Coord(50,30));
    Node& outside = insertNode(Coord(60,60));
    updateAreas();

    // Move 1
    Line l;
    l.push_back(Coord(10,60));
    l.push_back(Coord(20,60));
    l.push_back(Coord(20,30));

    doMove(l, Coord(20,50));
    Node& d = *nodes.back();

    // Move 2
    l.clear();
    l.push_back(Coord(20,50));
    l.push_back(Coord(50,50));
    l.push_back(Coord(50,30));

    doMove(l, Coord(50,40));
    Node& e = *nodes.back();

    // Move 3
    l.clear();
    l.push_back(Coord(20,30));
    l.push_back(Coord(20,20));
    l.push_back(Coord(35,20));
    l.push_back(Coord(35,40));
    l.push_back(Coord(50,40));

    doMove(l, Coord(30,20));
    Node& f = *nodes.back();

    // Move 4
    l.clear();
    l.push_back(Coord(30,20));
    l.push_back(Coord(30,10));
    l.push_back(Coord(50,10));
    l.push_back(Coord(50,30));
    
    doMove(l, Coord(40,10));
    Node& g = *nodes.back();

    // Test 'em
    passed = false;

    if (connectable(a,a) &&
        connectable(a,b) &&
        connectable(a,c) &&
        connectable(a,g) &&
        connectable(b,a) &&
        connectable(b,c) &&
        connectable(b,g) &&
        connectable(c,a) &&
        connectable(c,b) &&
        connectable(c,g) &&
        connectable(outside,a) &&
        connectable(outside,b) &&
        connectable(outside,c) &&
        connectable(outside,g) &&
        connectable(outside,outside) &&
        !connectable(a,d) &&
        !connectable(a,e) &&
        !connectable(a,f) &&
        !connectable(b,b) &&
        !connectable(b,d) &&
        !connectable(b,e) &&
        !connectable(b,f) &&
        !connectable(c,c) &&
        !connectable(c,d) &&
        !connectable(c,e) &&
        !connectable(c,f) &&
        !connectable(d,a) &&
        !connectable(d,b) &&
        !connectable(d,c) &&
        !connectable(d,d) &&
        !connectable(d,e) &&
        !connectable(d,f) &&
        !connectable(d,g) &&
        !connectable(e,a) &&
        !connectable(e,b) &&
        !connectable(e,c) &&
        !connectable(e,d) &&
        !connectable(e,e) &&
        !connectable(e,f) &&
        !connectable(e,g) &&
        !connectable(f,a) &&
        !connectable(f,b) &&
        !connectable(f,c) &&
        !connectable(f,d) &&
        !connectable(f,e) &&
        !connectable(f,f) &&
        !connectable(f,g) &&
        !connectable(g,d) &&
        !connectable(g,e) &&
        !connectable(g,f) &&
        !connectable(g,g) &&
        !connectable(outside,d) &&
        !connectable(outside,e) &&
        !connectable(outside,f))
        passed = true;

    test("game2", passed);
    //sdlRenderGame();
}
