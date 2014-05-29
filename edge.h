#include "node.h"
#ifndef EDGE_H
#define EDGE_H

#include <vector> //needed for list of cubic splines
#include <memory> //used for shared_ptr and weak_ptr

#include "cubicspline.h"

struct Region;

struct Edge
{
    vector<CubicSpline> cubicSplines; //list of cubic splines
    shared_ptr<Region> inside, outside;
    shared_ptr<Node> start, end;
    Edge(vector<CubicSpline> cubicSplines, shared_ptr<Node> start, shared_ptr<Node> end);
};

#include "region.h"

inline Edge::Edge(vector<CubicSpline> cubicSplines, shared_ptr<Node> start, shared_ptr<Node> end)
    : cubicSplines(cubicSplines), start(start), end(end)
{
    assert(cubicSplines[0].p0 == start->position);
    assert(cubicSplines.back().p1 == end->position);
}

#endif // EDGE_H
