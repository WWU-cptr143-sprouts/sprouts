#include <vector>
#include "structs.h"

#ifndef H_AREA
#define H_AREA
using namespace std;

typedef vector<Connection*> Area;
typedef vector<Area*> Areaset;
static const Areaset defaultAreaset; //create default/null vector
#endif
