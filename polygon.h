#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <vector>
#include "vector.h"

using namespace std;

typedef vector<VectorF> Polygon;

vector<Polygon> splitPolygon(Polygon poly);

inline bool isConvexPolygon(const Polygon & poly)
{
    if(poly.size() <= 3)
        return true;
    VectorF normal = cross(poly[0] - poly[1], poly[1] - poly[2]);
    for(size_t i = 1; i < poly.size(); i++)
    {
        size_t j = (i + 1) % poly.size();
        size_t k = (j + 1) % poly.size();
        if(dot(normal, cross(poly[i] - poly[j], poly[j] - poly[k])) <= 0)
            return false;
    }
    return true;
}

bool isPointInPolygon(const Polygon & poly, VectorF p); // in game_state.cpp

#endif // POLYGON_H_INCLUDED
