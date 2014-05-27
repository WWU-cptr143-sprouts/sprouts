#include "edge.h"
#ifndef REGION_H
#define REGION_H
#include <memory>
#include <list>
#include "graph.h"
#include "cubicspline.h"
#include "node.h"
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct Region
{
    list<weak_ptr<Edge>> edges;
    list<shared_ptr<Node>> nodes;
};
#endif // REGION_H
