#include "edge.h"
#ifndef REGION_H
#define REGION_H

#include <memory>
#include <vector>
#include "graph.h"
#include "cubicspline.h"
#include "node.h"
#include "polygon.h"

using namespace std;

struct Land
{
    vector<weak_ptr<Edge>> edges;
    vector<shared_ptr<Node>> nodes;
    bool isInverted;
    Polygon polygon;
};

inline Land transform(const Matrix & tform, Land land)
{
    land.polygon = transform(tform, std::move(land.polygon));
    return std::move(land);
}

struct Region
{
    vector<Land> lands;
    vector<shared_ptr<Node>> isolatedNodes;
};

inline Region transform(const Matrix & tform, Region region)
{
    for(Land & land : region.lands)
    {
        land = transform(tform, std::move(land));
    }
    return std::move(region);
}

struct DisjointPartition
{
    vector<weak_ptr<Node>> nodes;
    shared_ptr<Region> containingRegion;
};

#endif // REGION_H
