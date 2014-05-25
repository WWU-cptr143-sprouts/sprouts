#include "game_state.h"
#include "texture_atlas.h"

#if 0
bool operator ==(const GameState & l, const GameState & r)
{
#error finish
}
#else
#warning finish
#endif

#if 0
size_t std::hash<GameState>::operator ()(const GameState & gs)
{
#error finish
}
#else
#warning finish
#endif

#if 0
bool isValidGameState(const GameState & gs)
{
#error finish
}
#else
#warning finish
#endif

Mesh renderGameState(const GameState & gs)
{
    const float lineWidth = 0.01;
    const float dotSize = 0.02;
    Mesh retval = Mesh(new Mesh_t());
    for(auto nodeIterator = gs->begin(); nodeIterator != gs->end(); nodeIterator++)
    {
        shared_ptr<Node> node = *nodeIterator;
        Mesh mesh = Generate::line({node->position + VectorF(-dotSize, 0, -1), node->position + VectorF(dotSize, 0, -1)}, TextureAtlas::Point.td(), Color::V(1), dotSize);
        retval->add(transform(Matrix::scale(0.5), mesh));
        for(auto edgeIterator = gs->begin(nodeIterator); edgeIterator != gs->end(nodeIterator); edgeIterator++)
        {
            shared_ptr<Edge> edge = get<0>(*edgeIterator);
            auto mesh = transform(Matrix::translate(0, 0, -1), CubicSpline::renderSplineList(edge->cubicSplines, TextureAtlas::Line.td(), Color::V(1), lineWidth));
            retval->add(mesh);
        }
    }
    return retval;
}
