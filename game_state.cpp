#include "game_state.h"
#include "texture_atlas.h"
#include <unordered_map>
#include <unordered_set>

#if 0
bool operator ==(GameState l, GameState r)
{
#error finish
}
#else
#warning finish
#endif

#if 0
size_t std::hash<GameState>::operator ()(GameState gs)
{
#error finish
}
#else
#warning finish
#endif

const int edgePerNodeMax = 3;

bool isValidGameState(GameState gs)
{
    unordered_map<shared_ptr<Edge>, int> edgesMap;
    vector<shared_ptr<Edge>> edges;
    for(auto nodeIterator = gs->begin(); nodeIterator != gs->end(); nodeIterator++)
    {
        shared_ptr<Node> node = *nodeIterator;
        assert(node != nullptr);

        int edgeCount = 0;
        for(auto edgeIterator = gs->begin(nodeIterator); edgeIterator != gs->end(nodeIterator); edgeIterator++)
        {
            if(++edgeCount > edgePerNodeMax)
                return false;
            shared_ptr<Edge> edge = get<0>(*edgeIterator);
            assert(edge != nullptr);
            shared_ptr<Node> node2 = *get<1>(*edgeIterator);
            assert(node2 != nullptr);
            assert(edge->cubicSplines.size() > 0);
            if(edge->cubicSplines.front().p0 == node->position)
            {
                assert(edge->cubicSplines.front().p0 == node->position);
                assert(edge->cubicSplines.back().p1 == node2->position);
            }
            else
            {
                assert(edge->cubicSplines.front().p0 == node2->position);
                assert(edge->cubicSplines.back().p1 == node->position);
            }
            for(size_t i = 1; i < edge->cubicSplines.size(); i++)
            {
                assert(edge->cubicSplines[i - 1].p1 == edge->cubicSplines[i].p0);
                assert(edge->cubicSplines[i - 1].dp1 == edge->cubicSplines[i].dp0);
            }
            if(edgesMap.find(edge) == edgesMap.end())
            {
                edgesMap[edge] = 1; // theres one edge so far
                edges.push_back(edge);
            }
            else if(++edgesMap[edge] > 2)
            {
                assert(false);
            }
        }
    }
    for(auto i = edges.begin(); i != edges.end(); i++)
    {
        vector<CubicSpline> edge1 = (*i)->cubicSplines;
        const float adjustDistance = 0.01;
        edge1[0] = edge1[0].subSection(adjustDistance, 1);
        edge1[edge1.size() - 1] = edge1[edge1.size() - 1].subSection(0, 1 - adjustDistance);
        for(auto j = edges.begin(); j != i; j++)
        {
            const vector<CubicSpline> & edge2 = (*j)->cubicSplines;
            for(CubicSpline spline1 : edge2)
            {
                for(CubicSpline spline2 : edge1)
                {
                    if(spline1.intersects(spline2))
                        return false;
                }
            }
        }
    }
    return true;
}

Mesh renderGameState(GameState gs)
{
    const float lineWidth = 0.01;
    const float dotSize = 0.02;
    Mesh retval = Mesh(new Mesh_t());
    unordered_set<shared_ptr<Edge>> edges;
    for(auto nodeIterator = gs->begin(); nodeIterator != gs->end(); nodeIterator++)
    {
        shared_ptr<Node> node = *nodeIterator;
        Mesh mesh = Generate::line({node->position + VectorF(-dotSize, 0, -1), node->position + VectorF(dotSize, 0, -1)}, TextureAtlas::Point.td(), Color::V(1), dotSize);
        retval->add(transform(Matrix::scale(0.5), mesh));
        for(auto edgeIterator = gs->begin(nodeIterator); edgeIterator != gs->end(nodeIterator); edgeIterator++)
        {
            shared_ptr<Edge> edge = get<0>(*edgeIterator);
            if(get<1>(edges.insert(edge)))
            {
                auto mesh = transform(Matrix::translate(0, 0, -1), CubicSpline::renderSplineList(edge->cubicSplines, TextureAtlas::Line.td(), Color::V(1), lineWidth));
                retval->add(mesh);
            }
        }
    }
    return retval;
}
