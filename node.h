#ifndef NODE_H
#define NODE_H
#include <cstdlib>
#include "vector.h"
#include "graph.h"

using namespace std;

struct Edge;

struct Node //node structure
{
    VectorF position; //position of node in floating point vector
    graph<shared_ptr<Node>, shared_ptr<Edge>>::node_iterator iter;
    Node(VectorF position = VectorF())
        : position(position)
    {
    }
};

#include "edge.h"

#endif // NODE_H
