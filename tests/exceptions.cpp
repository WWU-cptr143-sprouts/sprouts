#include "tests.h"

void Tests::invalidCorner()
{
    bool passed = false;

    Node& a = insertNode(Coord(0,0));
    Node& b = insertNode(Coord(10,10));

    // Place a coordinate at (5,10), somehow split line into A->D and D->B
    Line AD_line;
    AD_line.push_back(Coord(0,0));
    AD_line.push_back(Coord(0,10));
    AD_line.push_back(Coord(5,10));
    Line DB_line;
    DB_line.push_back(Coord(5,10));
    DB_line.push_back(Coord(10,10));

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

    Line& DE = insertLine(DE_line);
    Line& EB = insertLine(EB_line);

    Node& e = insertNode(Coord(10, 30),
        Connection(&DE, &d),
        Connection(&EB, &b));
    
    d.addConnection(Connection(&DE, &e));

    try
    {
        b.addConnection(Connection(&EB, &e));
    }
    catch (const InvalidCorner& e)
    {
        passed = true;
    }
    catch (...)
    {
    }

    test("invalidCorners", passed);
}

void Tests::invalidLine()
{
    bool passed = false;

    Node& a = insertNode(Coord(0,0));

    Line AB_line;
    AB_line.push_back(Coord(0,0));
    AB_line.push_back(Coord(0,10));
    AB_line.push_back(Coord(5,10));
    Line& AB = insertLine(AB_line);

    // Attempt to make a connection with a line not going to the node
    try
    {
        insertNode(Coord(10,10),
            Connection(&AB, &a),
            Connection());
    }
    catch (const InvalidLine& e)
    {
        passed = true;
    }
    catch (...)
    {
    }

    test("invalidLine", passed);
}

void Tests::imageNotLoaded()
{
    bool passed = false;

    // Not sure if this is needed for the test
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Could not init SDL" << endl;
        return;
    }

    try
    {
        Image test("cow.png");
    }
    catch (const ImageNotLoaded& e)
    {
        passed = true;
    }
    catch (...)
    {
    }

    test("imageNotLoaded", passed);

    SDL_Quit();
}
