#include "game_state.h"
#include "texture_atlas.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>

using namespace std;

// Gamestate comparison function
#if 0
bool operator ==(GameState l, GameState r)
{
#error finish
}
#else
#warning finish
#endif

// Returns hash for current gamestate
#if 0
size_t std::hash<GameState>::operator()(GameState gs)
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

        for(auto edgeIterator = gs->begin(nodeIterator); edgeIterator != gs->end(nodeIterator);
                edgeIterator++)
        {
            if(++edgeCount > edgePerNodeMax)
            {
                return false;
            }

            shared_ptr<Edge> edge = get<0>(*edgeIterator);
            assert(edge != nullptr);
            shared_ptr<Node> node2 = *get<1>(*edgeIterator);
            assert(node2 != nullptr && node2 != node);
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
            const vector<CubicSpline> &edge2 = (*j)->cubicSplines;

            for(CubicSpline spline1 : edge2)
            {
                for(CubicSpline spline2 : edge1)
                {
                    if(spline1.intersects(spline2))
                    {
                        return false;
                    }
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
        Mesh mesh = Generate::line({node->position + VectorF(-dotSize, 0, -1), node->position + VectorF(dotSize, 0, -1)},
                                   TextureAtlas::Point.td(), Color::V(1), dotSize);
        retval->add(transform(Matrix::scale(0.5), mesh));

        for(auto edgeIterator = gs->begin(nodeIterator); edgeIterator != gs->end(nodeIterator);
                edgeIterator++)
        {
            shared_ptr<Edge> edge = get<0>(*edgeIterator);

            if(get<1>(edges.insert(edge)))
            {
                auto mesh = transform(Matrix::translate(0, 0, -1), CubicSpline::renderSplineList(edge->cubicSplines,
                                      TextureAtlas::Line.td(), Color::V(1), lineWidth));
                retval->add(mesh);
            }
        }
    }

    return retval;
}

namespace
{
struct Face;

struct MyEdge
{
    shared_ptr<Edge> edge;
    shared_ptr<Node> start;
    shared_ptr<Node> end;
    weak_ptr<Face> face;
    MyEdge(shared_ptr<Edge> edge, bool reversed)
        : edge(edge), start(reversed ? edge->end : edge->start), end(reversed ? edge->start : edge->end), face(shared_ptr<Face>(nullptr))
    {
    }
    MyEdge(shared_ptr<Edge> edge, shared_ptr<Node> start, shared_ptr<Node> end, shared_ptr<Face> face = nullptr)
        : edge(edge), start(start), end(end), face(face)
    {
    }
    friend bool operator ==(const MyEdge &l, const MyEdge &r)
    {
        return l.start == r.start && l.end == r.end;
    }
    friend bool operator !=(const MyEdge &l, const MyEdge &r)
    {
        return !operator ==(l, r);
    }
    MyEdge reversed() const
    {
        return MyEdge(edge, end, start);
    }
};

struct Face
{
    vector<MyEdge> edges;
    unordered_set<shared_ptr<Face>> interiorFaces;
    vector<shared_ptr<Face>> removeList;
    weak_ptr<Face> parent;
    Polygon polygon;
    bool isOutside;
    shared_ptr<Land> land;
    shared_ptr<Region> region;
};

Polygon getFacePolygon(shared_ptr<Face> face)
{
    const int splitCount = 20;
    assert(face);
    assert(face->edges.size() > 0);
    Polygon poly;
    poly.reserve(splitCount * face->edges.size() * 3);
    for(MyEdge myEdge : face->edges)
    {
        shared_ptr<Edge> edge = myEdge.edge;
        if(edge->start == myEdge.start) // if the edge is forward
        {
            for(size_t i = 0; i < edge->cubicSplines.size(); i++)
            {
                CubicSpline spline = edge->cubicSplines[i];
                for(int j = 0; j < splitCount; j++)
                {
                    float t = (float)j / splitCount;
                    poly.push_back(spline.evaluate(t));
                }
            }
        }
        else // the edge is reversed
        {
            for(ptrdiff_t i = edge->cubicSplines.size() - 1; i >= 0; i--)
            {
                CubicSpline spline = edge->cubicSplines[i];
                for(int j = 0; j < splitCount; j++)
                {
                    float t = 1 - (float)j / splitCount;
                    poly.push_back(spline.evaluate(t));
                }
            }
        }
    }
    return poly;
}

float getPseudoAngle(VectorF v) // doesn't really return the angle : returns a number that sorts in the same order but can be calculated faster
{
    v.z = 0;

    if(v == VectorF(0))
    {
        return 0;
    }

    v /= max(abs(v.x + v.y), abs(v.x - v.y));

    if(v.y > 0)
    {
        return 1 - v.x;
    }

    return 3 + v.x;
}

/** \brief returns a number that sorts in the same order as angles but can be calculated faster
 *
 * \param the edge
 * \return the pseudo-angle
 *
 * \note doesn't really return the angle : returns a number that sorts in the same order but can be calculated faster
 */
float getEdgePseudoAngle(MyEdge v)
{
    auto edge = v.edge;
    auto node = v.start;
    CubicSpline spline = edge->cubicSplines.front();

    if(node->position != spline.p0)
    {
        spline = edge->cubicSplines.back().reversed();
    }

    VectorF cubic = spline.getCubic();
    VectorF quadratic = spline.getQuadratic();
    VectorF linear = spline.getLinear();

    if(absSquared(linear) > eps * eps)
    {
        return getPseudoAngle(linear);
    }

    if(absSquared(quadratic) > eps * eps)
    {
        return getPseudoAngle(quadratic);
    }

    return getPseudoAngle(cubic);
}

float getAngle(VectorF v)
{
    v.z = 0;

    if(v == VectorF(0))
    {
        return 0;
    }

    float angle = atan2(v.y, v.x);
    if(angle >= 2 * M_PI)
        angle -= 2 * M_PI;
    if(angle < 0)
        angle += 2 * M_PI;
    return angle;
}

float getEdgeAngle(MyEdge v)
{
    auto edge = v.edge;
    auto node = v.start;
    CubicSpline spline = edge->cubicSplines.front();

    if(node->position != spline.p0)
    {
        spline = edge->cubicSplines.back().reversed();
    }

    VectorF cubic = spline.getCubic();
    VectorF quadratic = spline.getQuadratic();
    VectorF linear = spline.getLinear();

    if(absSquared(linear) > eps * eps)
    {
        return getAngle(linear);
    }

    if(absSquared(quadratic) > eps * eps)
    {
        return getAngle(quadratic);
    }

    return getAngle(cubic);
}

float getAngleDelta(MyEdge a, MyEdge b)
{
    float angle = getEdgeAngle(a) - getEdgeAngle(b);
    if(angle >= 2 * M_PI)
        angle -= 2 * M_PI;
    if(angle < 0)
        angle += 2 * M_PI;
    return angle;
}
}

namespace std
{
template <>
struct hash<MyEdge>
{
    size_t operator()(const MyEdge &edge) const
    {
        return hash<shared_ptr<Node>>()(edge.start) + hash<shared_ptr<Node>>()(edge.start);
    }
};
}

/**
 * recalculate regions in the graph
 *
 * @note part of this algorithm is from http://www.sagemath.org/doc/reference/graphs/sage/graphs/generic_graph.html#sage.graphs.generic_graph.GenericGraph.faces
 */
void recalculateRegions(GameState gs)
{
    assert(gs);
    unordered_set<MyEdge> edges, edgesLeft;
    unordered_map<shared_ptr<Node>, vector<MyEdge>> neighborsMap;
    vector<MyEdge> path;
    vector<shared_ptr<Face>> faces;
    unordered_map<MyEdge, shared_ptr<Face>> facesMap;
    vector<shared_ptr<Node>> isolatedNodes;

    for(auto ni = gs->begin(); ni != gs->end(); ni++)
    {
        auto node = *ni;
        assert(node);
        node->iter = ni;
        vector<MyEdge> neighbors;

        for(auto ei = gs->begin(ni); ei != gs->end(ni); ei++)
        {
            auto edge = get<0>(*ei);
            assert(edge);
            auto destNode = *get<1>(*ei);
            assert(destNode);
            assert(edge->cubicSplines.size() > 0);

            if(edge->cubicSplines[0].p0 == node->position)
            {
                edge->start = node;
                edge->end = destNode;
            }
            else
            {
                edge->start = destNode;
                edge->end = node;
            }

            neighbors.push_back(MyEdge(edge, node, destNode));
            edges.insert(MyEdge(edge, false));
            edges.insert(MyEdge(edge, true));
        }

        sort(neighbors.begin(), neighbors.end(), [](MyEdge a, MyEdge b)
        {
            return getEdgePseudoAngle(a) < getEdgePseudoAngle(b);
        });
        neighborsMap[node] = neighbors;
        if(neighbors.size() == 0)
            isolatedNodes.push_back(node);
    }

    edgesLeft = edges;

    if(!edgesLeft.empty())
    {
        path.push_back(*edgesLeft.begin());
        edgesLeft.erase(edgesLeft.begin());
    }

    // find all faces
    while(!edgesLeft.empty())
    {
        auto &neighbors = neighborsMap[path.back().end];
        auto curEdge = find_if(neighbors.begin(), neighbors.end(),
                                           [&path](MyEdge e) -> bool
        {
            return path.back().start == e.end;
        });
        assert(curEdge != neighbors.end());

        if(++curEdge == neighbors.end())
        {
            curEdge = neighbors.begin();
        }

        auto theEdge = *curEdge;
        if(theEdge == path.front())
        {
            shared_ptr<Face> face = make_shared<Face>();
            face->edges = std::move(path);
            for(auto edge : face->edges)
                facesMap[edge] = face;
            faces.push_back(face);
            path.clear();
            if(!edgesLeft.empty())
            {
                path.push_back(*edgesLeft.begin());
                edgesLeft.erase(edgesLeft.begin());
            }
        }
        else
        {
            path.push_back(theEdge);
            edgesLeft.erase(theEdge);
        }
    }

    if(!path.empty())
    {
        shared_ptr<Face> face = make_shared<Face>();
        face->edges = std::move(path);
        for(auto edge : face->edges)
            facesMap[edge] = face;
        faces.push_back(face);
    }

    // compute face attributes

    for(shared_ptr<Face> face : faces)
    {
        float angleSum = 0;
        MyEdge lastEdge = face->edges.back();
        for(MyEdge edge : face->edges)
        {
            angleSum += getAngleDelta(lastEdge, edge);
            lastEdge = edge;
        }
        face->isOutside = false;
        if(angleSum < 0)
            face->isOutside = true;
        face->polygon = getFacePolygon(face);
    }

    // calculate interiorFaces

    for(shared_ptr<Face> face : faces)
    {
        if(face->isOutside)
        {
            for(MyEdge edge : face->edges)
            {
                shared_ptr<Face> interiorFace = facesMap[edge.reversed()];
                if(interiorFace != face)
                    face->interiorFaces.insert(interiorFace);
            }
        }
        else
        {
            for(MyEdge edge : edges)
            {
                bool canTest = true;
                for(MyEdge edge2 : face->edges)
                {
                    if(edge.start == edge2.start || edge.start == edge2.end || edge.end == edge2.start || edge.end == edge2.end)
                    {
                        canTest = false;
                        break;
                    }
                }
                if(!canTest)
                    continue;
                if(!isPointInPolygon(face->polygon, edge.start->position))
                    continue;
                shared_ptr<Face> interiorFace = facesMap[edge];
                face->interiorFaces.insert(interiorFace);
            }
        }
    }

    // convert to a tree and create lands

    for(shared_ptr<Face> face : faces)
    {
        face->removeList.reserve(faces.size());
        for(shared_ptr<Face> interiorFace : face->interiorFaces)
        {
            copy(interiorFace->interiorFaces.begin(), interiorFace->interiorFaces.end(), back_inserter(face->removeList));
        }
        face->parent = shared_ptr<Face>(nullptr);
    }

    for(shared_ptr<Face> face : faces)
    {
        for(shared_ptr<Face> removeFace : face->removeList)
        {
            face->interiorFaces.erase(removeFace);
        }
        for(shared_ptr<Face> interiorFace : face->interiorFaces)
        {
            interiorFace->parent = face;
        }
        face->land = make_shared<Land>();
        face->land->isInverted = face->isOutside;
        face->land->polygon = face->polygon;
        for(MyEdge edge : face->edges)
        {
            face->land->edges.push_back(edge.edge);
            face->land->nodes.push_back(edge.start);
        }
    }

    // convert faces found into regions

    vector<shared_ptr<Region>> regions;
    shared_ptr<Region> outsideRegion = make_shared<Region>();
    regions.push_back(outsideRegion);
    for(shared_ptr<Face> face : faces)
    {
        if(!face->parent.lock())
        {
            outsideRegion->lands.push_back(*face->land);
            face->region = outsideRegion;
            continue;
        }
        shared_ptr<Face> regionFace = face;
        if(regionFace->isOutside)
        {
            regionFace = shared_ptr<Face>(regionFace->parent);
        }
        if(regionFace->region == nullptr)
        {
            regionFace->region = make_shared<Region>();
            regions.push_back(regionFace->region);
        }
        face->region = regionFace->region;
        face->region->lands.push_back(*face->land);
    }

    // set region pointers in edges

    for(MyEdge edge : edges)
    {
        if(edge.start == edge.edge->start)
        {
            edge.edge->inside = facesMap[edge]->region;
        }
        else
        {
            edge.edge->outside = facesMap[edge]->region;
        }
    }

    // add isolated nodes to regions

    for(shared_ptr<Node> node : isolatedNodes)
    {
        for(shared_ptr<Region> region : regions)
        {
            if(isPointInRegion(region, node->position))
            {
                region->isolatedNodes.push_back(node);
                break;
            }
        }
    }
}

bool isPointInPolygon(const Polygon & poly, VectorF p)
{
    assert(poly.size() >= 3);
    size_t hitCount = 0;
    for(size_t i = 0; i < poly.size(); i++)
    {
        size_t j = (i + 1) % poly.size();
        VectorF delta = poly[j] - poly[i];
        assert(absSquared(delta) > eps * eps);
        float t = (poly[i].y - p.y) / delta.y;
        if(t <= 0 || t > 1)
            continue;
        VectorF hitPt = poly[i] + t * delta;
        if(hitPt.x < p.x)
            continue;
        hitCount++;
    }
    return (hitCount % 2 == 0);
}

Polygon getLandPolygon(const Land &land)
{
    const int splitCount = 20;
    assert(land.edges.size() > 0);
    Polygon poly;
    poly.reserve(splitCount * land.edges.size() * 3);
    VectorF lastPt = shared_ptr<Edge>(land.edges[0])->cubicSplines[0].p0;
    for(weak_ptr<Edge> wedge : land.edges)
    {
        shared_ptr<Edge> edge = shared_ptr<Edge>(wedge);
        VectorF startPt = edge->cubicSplines[0].p0;
        VectorF endPt = edge->cubicSplines.back().p1;
        if(absSquared(startPt - lastPt) < absSquared(endPt - lastPt)) // if the edge is forward
        {
            for(size_t i = 0; i < edge->cubicSplines.size(); i++)
            {
                CubicSpline spline = edge->cubicSplines[i];
                for(int j = 0; j < splitCount; j++)
                {
                    float t = (float)j / splitCount;
                    poly.push_back(spline.evaluate(t));
                }
            }
            lastPt = endPt;
        }
        else // the edge is reversed
        {
            for(ptrdiff_t i = edge->cubicSplines.size() - 1; i >= 0; i--)
            {
                CubicSpline spline = edge->cubicSplines[i];
                for(int j = 0; j < splitCount; j++)
                {
                    float t = 1 - (float)j / splitCount;
                    poly.push_back(spline.evaluate(t));
                }
            }
            lastPt = startPt;
        }
    }
    return poly;
}

bool isPointInRegion(shared_ptr<Region> r, VectorF p)
{
    bool isOutsideRegion = true;
    assert(r);
    for(Land land : r->lands)
    {
        if(!land.isInverted)
            isOutsideRegion = false;
        if(isPointInPolygon(land.polygon, p))
        {
            if(land.isInverted)
                return false;
        }
        else if(!land.isInverted)
        {
            return false;
        }
    }
    if(isOutsideRegion)
        return true;
    else
        return false;
}

#if 1
#warning finish implementing splitPolygon
#else
vector<Polygon> splitPolygon(Polygon poly)
{
    if(poly.size() <= 3)
        return vector<Polygon>(1, poly);
    vector<Polygon> retval;
    do
    {
        retval.push_back(Polygon());
        int polyIndex = 0;
        retval.back().push_back(poly[polyIndex++]);
        retval.back().push_back(poly[polyIndex++]);
        retval.back().push_back(poly[polyIndex++]); // push 3 points for a triangle
        for(;;)
        {
            retval.back().push_back(poly[polyIndex]);
            if(isConvexPolygon(retval.back()))
        }
    }
    while(poly.size() > 0);
    return retval;
}
#endif

GameState duplicate(GameState gs)
{
    GameState retval = makeEmptyGameState();
    unordered_map<shared_ptr<Node>, shared_ptr<Node>> nodeMap;
    unordered_map<shared_ptr<Edge>, shared_ptr<Edge>> edgeMap;
    unordered_set<shared_ptr<Edge>> edges;
    for(auto ni = gs->begin(); ni != gs->end(); ni++)
    {
        auto node = *ni;
        auto newNode = make_shared<Node>(node->position);
        nodeMap[node] = newNode;
        newNode->iter = retval->addNode(newNode);
        for(auto ei = gs->begin(ni); ei != gs->end(ni); ei++)
        {
            shared_ptr<Edge> edge = get<0>(*ei);
            edges.insert(edge);
        }
    }
    for(shared_ptr<Edge> edge : edges)
    {
        auto newEdge = make_shared<Edge>(edge->cubicSplines, nodeMap[edge->start], nodeMap[edge->end]);
        retval->addEdge(newEdge, newEdge->start->iter, newEdge->end->iter);
        retval->addEdge(newEdge, newEdge->end->iter, newEdge->start->iter);
    }
    recalculateRegions(retval);
    return retval;
}
