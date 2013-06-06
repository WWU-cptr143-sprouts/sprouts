#ifndef H_AREA
#define H_AREA

#include <cmath>
#include <vector>
#include <iostream>
#include "structs.h"

using namespace std;

typedef vector<Connection*> Area;
typedef vector<Area*> Areaset;

// This should really be constant, but we have to add it's address to a
// non-const vector
static Areaset defaultAreaset; //create default/null vector

ostream& operator<<(ostream& os, const Area& a);

// We need to write tests before using this.
bool operator==(const Area& a,const Area& b);
bool operator!=(const Area& a,const Area& b);
#endif
