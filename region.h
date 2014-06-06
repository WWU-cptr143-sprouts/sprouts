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

struct Region
{
    vector<Land> lands;
    vector<shared_ptr<Node>> isolatedNodes;
};

struct DisjointPartition
{
    vector<weak_ptr<Node>> nodes;
    shared_ptr<Region> containingRegion;
};

#endif // REGION_H
