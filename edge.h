#ifndef EDGE_H
#define EDGE_H

#include <list> //needed for list of cubic splines
#include <memory> //used for shared_ptr and weak_ptr

#include "cubicspline.h"

struct Region;

struct Edge
{
    list<CubicSpline> cubicSplines; //list of cubic splines
    shared_ptr<Region> inside, outside;
};

#include "region.h"

#endif // EDGE_H
