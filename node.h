#ifndef NODE_H
#define NODE_H
#include <cstdlib>
#include "vector.h"
#include "graph.h"

using namespace std;

struct Edge;
struct DisjointPartition;

struct Node //node structure
{
    VectorF position; //position of node in floating point vector
    graph<shared_ptr<Node>, shared_ptr<Edge>>::node_iterator iter;
    shared_ptr<DisjointPartition> partition;
    Node(VectorF position = VectorF())
        : position(position)
    {
    }
};

#include "region.h"
#include "edge.h"

#endif // NODE_H
