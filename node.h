#ifndef NODE_H
#define NODE_H
#include <cstdlib>
#include "vector.h"
#include "graph.h"
#include "matrix.h"

using namespace std;

/**************
 *Description:
 *Input:
 *Output:
**************/

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

inline Node transform(const Matrix &tform, Node node)
{
    node.position = tform.apply(node.position);
    return std::move(node);
}

#include "region.h"
#include "edge.h"

#endif // NODE_H
