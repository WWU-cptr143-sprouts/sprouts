#ifndef NODE_H
#define NODE_H
#include <cstdlib>
#include "vector.h"
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct Node //node structure
{
    VectorF position; //position of node in floating point vector
    Node(VectorF position = VectorF())

        /**
         * @brief Write what the function does here
         *
         * @param position
         *
         * @return
         **/
        : position(position)
        {}
};
#endif // NODE_H
