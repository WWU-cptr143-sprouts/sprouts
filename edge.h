#include "node.h"
#ifndef EDGE_H
#define EDGE_H

#include <vector> //needed for list of cubic splines
#include <memory> //used for shared_ptr and weak_ptr
#include <cassert>

/**************
/*Description:
/*Input:
/*Output:
**************/

using namespace std;

#include "cubicspline.h"

struct Region;

struct Edge
{
    vector<CubicSpline> cubicSplines; //list of cubic splines
    shared_ptr<Region> inside, outside;
    shared_ptr<Node> start, end;
    Edge(const vector<CubicSpline> & cubicSplines, shared_ptr<Node> start, shared_ptr<Node> end); // in game_state.cpp
};

inline Edge transform(const Matrix &tform, Edge edge)
{
    edge.cubicSplines = transform(tform, std::move(edge.cubicSplines));
    return std::move(edge);
}

#include "region.h"

#endif // EDGE_H
