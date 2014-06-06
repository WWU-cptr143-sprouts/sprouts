#ifndef GAME_STATE_H_INCLUDED
#define GAME_STATE_H_INCLUDED

#include <memory>
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "mesh.h"
#include "generate.h"
#include "polygon.h"
#include <functional>
#include <stack>

using namespace std;

//Declaring the type GameState to contain what we need to know about the current game's state
//In this case a graph of nodes and edges from which we can find regions/etc
typedef shared_ptr<graph<shared_ptr<Node>, shared_ptr<Edge>>> GameState;

//At the start of the game we need to make an empty game state we can then add things to
inline GameState makeEmptyGameState()
{
    return GameState(new graph<shared_ptr<Node>, shared_ptr<Edge>>());
}

//Overloading operators to compare game states
bool operator ==(GameState l, GameState r);
inline bool operator !=(GameState l, GameState r)
{
    return !operator ==(l, r);
}

//Adding some things to the standard game state so our whole project can use them
//A bit dangerous but should work fine for what we're doing
//We implement here a simple hash function based on the size of the GameState
namespace std
{
template <>
struct hash<GameState>
{
    size_t operator ()(const GameState & gs);
};
}

//Get the polygon of the land, in order to find regions
Polygon getLandPolygon(const Land &land);

//Check if a certain point is in a certain region
bool isPointInRegion(shared_ptr<Region> r, VectorF p);

//Convert a point to the region it is in
shared_ptr<Region> pointToRegion(GameState gs, VectorF p);

//Recalculate our current regions, needs to be done after every move
void recalculateRegions(GameState gs);

//Check if the current game state is actually legal
bool isValidGameState(GameState gs, bool ignoreNodeCounts = false);

//Render a node to display it on screen, set the color to V(0.5)
TransformedMesh renderNode(shared_ptr<Node> node, Color color = Color::V(0.5));

//Display what the game currently looks like for the player
Mesh renderGameState(GameState gs);

//Duplicate our current game state, so we can mess with it without changing the original
GameState duplicate(GameState gs, vector<shared_ptr<Node> *> translateList = vector<shared_ptr<Node> *>());

//Define a GameState stack so we can go back moves if we need to
typedef stack<GameState> GameStateStack;

//Return the game state after making a move, passes it the current game state as well as move information
GameState move(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode, const vector<CubicSpline> & path); // returns nullptr if it is an invalid move

//Find the closest node in the current game state to a particular point
inline shared_ptr<Node> findClosestNode(GameState gs, VectorF p, float maxDistance = 0.02)
{
    shared_ptr<Node> retval = nullptr;
    float distSquared = maxDistance * maxDistance;
    for(shared_ptr<Node> node : *gs)
    {
        float curDistSquared = absSquared(node->position - p);
        if(curDistSquared < distSquared)
        {
            distSquared = curDistSquared;
            retval = node;
        }
    }
    return retval;
}

#endif // GAME_STATE_H_INCLUDED
