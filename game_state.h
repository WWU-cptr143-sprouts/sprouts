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
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

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

GameState duplicate(GameState gs, vector<shared_ptr<Node> *> translateNodeList = vector<shared_ptr<Node> *>(), vector<shared_ptr<Edge> *> translateEdgeList = vector<shared_ptr<Edge> *>());

typedef stack<GameState> GameStateStack;

struct InvalidMoveException final : public runtime_error
{
    explicit InvalidMoveException()
        : runtime_error("invalid move")
    {
    }
};

class GameStateMove final
{
private:
    GameState initialState, finalState;
    shared_ptr<Node> startNode;
    shared_ptr<Node> endNode;
    vector<shared_ptr<DisjointPartition>> insidePartitions;
    vector<shared_ptr<DisjointPartition>> outsidePartitions;
    shared_ptr<Region> containingRegion;
    void calculateFinalState();
public:
    GameStateMove(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode, const vector<CubicSpline> & path);
    GameStateMove(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode, vector<shared_ptr<DisjointPartition>> insidePartitions, vector<shared_ptr<DisjointPartition>> outsidePartitions, shared_ptr<Region> containingRegion);
    operator GameState()
    {
        if(finalState == nullptr)
            calculateFinalState();
        return finalState;
    }
};

inline int validMoveCount(GameState gs)
{
    unordered_map<shared_ptr<Region>, unordered_set<shared_ptr<Node>>> regions;
    unordered_set<shared_ptr<DisjointPartition>> partitions;
    unordered_map<shared_ptr<Node>, int> nodeEdgeCount;
    for(auto ni = gs->begin(); ni != gs->end(); ni++)
    {
        auto node = *ni;
        partitions.insert(node->partition);
        int & curNodeEdgeCount = nodeEdgeCount[node];
        curNodeEdgeCount = 0;
        for(auto ei = gs->begin(ni); ei != gs->end(ni); ei++)
        {
            auto edge = get<0>(*ei);
            regions[edge->inside].insert(node);
            regions[edge->inside].insert(*get<1>(*ei));
            regions[edge->outside].insert(node);
            regions[edge->outside].insert(*get<1>(*ei));
            curNodeEdgeCount++;
        }
    }
    for(auto &v : regions)
    {
        auto region = get<0>(v);
        for(auto node : region->isolatedNodes)
        {
            get<1>(v).insert(node);
        }
    }
    int retval = 0;
    if(regions.empty())
    {
        return gs->nodeCount() * gs->nodeCount();
    }
    for(auto &v : regions)
    {
        shared_ptr<Region> region = get<0>(v);
        unordered_set<shared_ptr<Node>> & nodes = get<1>(v);
        for(shared_ptr<Node> startNode : nodes)
        {
            if(nodeEdgeCount[startNode] >= 3)
                continue;
            for(shared_ptr<Node> endNode : nodes)
            {
                if(endNode == startNode && nodeEdgeCount[endNode] >= 2)
                    continue;
                else if(nodeEdgeCount[endNode] >= 3)
                    continue;
                if(endNode->partition != startNode->partition)
                {
                    retval++;
                    continue;
                }
                int availablePartitionCount = 0;
                for(auto partition : partitions)
                {
                    if(partition->containingRegion != region)
                        continue;
                    if(partition == startNode->partition)
                        continue;
                    assert(partition != endNode->partition);
                    availablePartitionCount++;
                }
                retval += 1 << availablePartitionCount;
            }
        }
    }
    return retval;
}

inline GameState transform(const Matrix & tform, GameState gs)
{
    unordered_set<shared_ptr<Edge>> edges;
    unordered_set<shared_ptr<Region>> regions;
    for(shared_ptr<Node> node : *gs)
    {
        *node = transform(tform, std::move(*node));
    }
    for(auto ni = gs->begin(); ni != gs->end(); ni++)
    {
        for(auto ei = gs->begin(ni); ei != gs->end(ni); ei++)
        {
            edges.insert(get<0>(*ei));
        }
    }
    for(shared_ptr<Edge> edge : edges)
    {
        regions.insert(edge->inside);
        regions.insert(edge->outside);
        *edge = transform(tform, std::move(*edge));
    }
    for(shared_ptr<Region> region : regions)
    {
        *region = transform(tform, std::move(*region));
    }
    return gs;
}

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
