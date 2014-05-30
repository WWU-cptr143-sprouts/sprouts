#ifndef GAME_STATE_H_INCLUDED
#define GAME_STATE_H_INCLUDED

#include <memory>
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "mesh.h"
#include "generate.h"
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

typedef vector<VectorF> Polygon;

Polygon getRegionPolygon(shared_ptr<Region> r);

vector<Polygon> splitPolygon(Polygon poly);

inline bool isConvexPolygon(const Polygon & poly)
{
    if(poly.size() <= 3)
        return true;
    VectorF normal = cross(poly[0] - poly[1], poly[1] - poly[2]);
    for(size_t i = 1; i < poly.size(); i++)
    {
        size_t j = (i + 1) % poly.size();
        size_t k = (j + 1) % poly.size();
        if(dot(normal, cross(poly[i] - poly[j], poly[j] - poly[k])) <= 0)
            return false;
    }
    return true;
}

bool isPointInRegion(shared_ptr<Region> r, VectorF p);

bool isPointInPolygon(const Polygon & poly, VectorF p);

void recalculateRegions(GameState gs);

bool isValidGameState(GameState gs);

Mesh renderGameState(GameState gs);

typedef stack<GameState> GameStateStack;

#endif // GAME_STATE_H_INCLUDED
