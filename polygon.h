#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <vector>
#include "vector.h"
#include "matrix.h"
#include "cubicspline.h"

/**************
 *Description:
 *Input:
 *Output:
**************/

using namespace std;

typedef vector<VectorF> Polygon;

inline Polygon transform(const Matrix & tform, Polygon poly)
{
    for(VectorF & p : poly)
    {
        p = tform.apply(p);
    }
    return std::move(poly);
}

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

inline bool isNonComplex(const Polygon & poly)
{
    for(size_t i = 1; i < poly.size(); i++)
    {
        size_t i2 = (i + 1) % poly.size();
        for(size_t j = 0; j < i; j++)
        {
            size_t j2 = (j + 1) % poly.size();
            if(linesIntersect(poly[i], poly[i2], poly[j], poly[j2]))
                return false;
        }
    }
    return true;
}

inline Polygon despikePolygon(Polygon poly)
{
    for(size_t i = 0; i < poly.size();) // remove duplicates
    {
        size_t j = (i + 1) % poly.size();
        if(absSquared(poly[i] - poly[j]) < eps * eps)
            poly.erase(poly.begin() + i);
        else
            i++;
    }
    bool anyChange = true;
    while(anyChange)
    {
        anyChange = false;
        for(size_t i = 0; i < poly.size();) // remove spikes
        {
            size_t j = (i + 1) % poly.size();
            size_t k = (i + 2) % poly.size();
            if(i == k)
                break;
            if(absSquared(cross(poly[i] - poly[j], poly[j] - poly[k])) < eps * eps * eps * eps)
            {
                if(absSquared(poly[i] - poly[k]) < eps * eps)
                {
                    poly.erase(poly.begin() + i, poly.begin() + j);
                }
                else
                {
                    poly.erase(poly.begin() + j);
                }
                anyChange = true;
            }
            else
                i++;
        }
        if(poly.size() < 3)
            return Polygon();
    }
    return poly;
}

bool isPointInPolygon(const Polygon & poly, VectorF p); // in game_state.cpp

#endif // POLYGON_H_INCLUDED
