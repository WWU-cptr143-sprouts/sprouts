#ifndef EDGE_H
#define EDGE_H
#include <vector> //needed for list of cubic splines
#include <memory> //used for shared_ptr and weak_ptr
#include "cubicspline.h"
struct Region;

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
struct Edge
{
    vector<CubicSpline> cubicSplines; //list of cubic splines
    shared_ptr<Region> inside, outside;
    Edge(vector<CubicSpline> cubicSplines, shared_ptr<Region> inside, shared_ptr<Region> outside)
        : cubicSplines(cubicSplines), inside(inside), outside(outside)
    {
    }
};
#include "region.h"
#endif // EDGE_H
