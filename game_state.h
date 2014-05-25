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

bool operator ==(const GameState & l, const GameState & r);
inline bool operator !=(const GameState & l, const GameState & r)
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

bool isValidGameState(const GameState & gs);

Mesh renderGameState(const GameState & gs);

typedef stack<GameState> GameStateStack;

#endif // GAME_STATE_H_INCLUDED
