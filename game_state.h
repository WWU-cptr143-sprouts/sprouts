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

typedef shared_ptr<graph<shared_ptr<Node>, shared_ptr<Edge>>> GameState;

inline GameState makeEmptyGameState()
{
    return GameState(new graph<shared_ptr<Node>, shared_ptr<Edge>>());
}

bool operator ==(GameState l, GameState r);
inline bool operator !=(GameState l, GameState r)
{
    return !operator ==(l, r);
}

namespace std
{
template <>
struct hash<GameState>
{
    size_t operator ()(const GameState & gs);
};
}

Polygon getLandPolygon(const Land &land);

bool isPointInRegion(shared_ptr<Region> r, VectorF p);

shared_ptr<Region> pointToRegion(GameState gs, VectorF p);

void recalculateRegions(GameState gs);

bool isValidGameState(GameState gs, bool ignoreNodeCounts = false);

TransformedMesh renderNode(shared_ptr<Node> node, Color color = Color::V(0.5));

Mesh renderGameState(GameState gs);

GameState duplicate(GameState gs, vector<shared_ptr<Node> *> translateList = vector<shared_ptr<Node> *>());

typedef stack<GameState> GameStateStack;

GameState move(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode, const vector<CubicSpline> & path); // returns nullptr if it is an invalid move

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
