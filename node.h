#ifndef NODE_H
#define NODE_H
#include <cstdlib>
#include "vector.h"
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
struct Node //node structure
{
    VectorF position; //position of node in floating point vector
    Node(VectorF position = VectorF())
        : position(position)
    {}
};
#endif // NODE_H
