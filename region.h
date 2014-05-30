#include "edge.h"
#ifndef REGION_H
#define REGION_H

#include <memory>
#include <vector>
#include "graph.h"
#include "cubicspline.h"
#include "node.h"

using namespace std;

struct Region
{
    vector<weak_ptr<Edge>> edges;
    vector<shared_ptr<Node>> nodes;
    bool isOutsideRegion;
};

#endif // REGION_H
