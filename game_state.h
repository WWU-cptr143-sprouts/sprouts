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

/**
 * @brief Write what the function does here
 *
 * @return
 **/
inline GameState makeEmptyGameState()
{
    return GameState(new graph<shared_ptr<Node>, shared_ptr<Edge>>());
}
bool operator ==(GameState l, GameState r);

/**
 * @brief Write what the function does here
 *
 * @param l
 * @param r
 *
 * @return
 **/
inline bool operator !=(GameState l, GameState r)
{
    return !operator ==(l, r);
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
namespace std
{
    template <>

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        struct hash<GameState>
        {
            size_t operator ()(const GameState & gs);
        };
}
bool isValidGameState(GameState gs);
Mesh renderGameState(GameState gs);
typedef stack<GameState> GameStateStack;
#endif // GAME_STATE_H_INCLUDED
