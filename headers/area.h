#ifndef H_AREA
#define H_AREA

#include <vector>
#include "structs.h"

using namespace std;

typedef vector<Connection*> Area;
typedef vector<Area*> Areaset;

// This should really be constant, but we have to add it's address to a
// non-const vector
static Areaset defaultAreaset; //create default/null vector

#endif
