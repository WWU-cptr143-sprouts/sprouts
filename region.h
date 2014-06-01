#include "edge.h"
#ifndef REGION_H
#define REGION_H

#include <memory>
#include <vector>
#include "graph.h"
#include "cubicspline.h"
#include "node.h"

using namespace std;

struct Land
{
    vector<weak_ptr<Edge>> edges;
    bool isInverted;
};

struct Region
{
    vector<Land> lands;
    vector<shared_ptr<Node>> nodes;
    bool isOutsideRegion;
};

#endif // REGION_H
