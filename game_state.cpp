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

bool isValidGameState(GameState gs, bool ignoreNodeCounts)
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
            if(++edgeCount > edgePerNodeMax && !ignoreNodeCounts)
            {
                return false;
            }

            shared_ptr<Edge> edge = get<0>(*edgeIterator);
            assert(edge != nullptr);
            shared_ptr<Node> node2 = *get<1>(*edgeIterator);
            assert(node2 != nullptr && node2 != node);
            assert(edge->cubicSplines.size() > 0);

            if(absSquared(edge->cubicSplines.front().p0 - node->position) < eps * eps)
            {
                assert(absSquared(edge->cubicSplines.front().p0 - node->position) < eps * eps);
                assert(absSquared(edge->cubicSplines.back().p1 - node2->position) < eps * eps);
            }
            else
            {
                assert(absSquared(edge->cubicSplines.front().p0 - node2->position) < eps * eps);
                assert(absSquared(edge->cubicSplines.back().p1 - node->position) < eps * eps);
            }

            for(size_t i = 1; i < edge->cubicSplines.size(); i++)
            {
                assert(absSquared(edge->cubicSplines[i - 1].p1 - edge->cubicSplines[i].p0) < eps * eps);
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
        const float adjustDistance = 0.1;
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

TransformedMesh renderNode(shared_ptr<Node> node, Color color)
{
    const float dotSize = 0.02;
    Mesh mesh = Generate::line({node->position + VectorF(-dotSize, 0, -1), node->position + VectorF(dotSize, 0, -1)},
                               TextureAtlas::Point.td(), color, dotSize);
    return transform(Matrix::scale(0.5), mesh);
}

Mesh renderGameState(GameState gs)
{
    const float lineWidth = 0.01;
    Mesh retval = Mesh(new Mesh_t());
    unordered_set<shared_ptr<Edge>> edges;

    for(auto nodeIterator = gs->begin(); nodeIterator != gs->end(); nodeIterator++)
    {
        shared_ptr<Node> node = *nodeIterator;
        retval->add(renderNode(node));

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

VectorF getEdgeAngleVector(MyEdge v)
{
    auto edge = v.edge;
    auto node = v.start;
    CubicSpline spline = edge->cubicSplines.front();

    if(node->position != spline.p0)
    {
        spline = edge->cubicSplines.back().reversed();
    }
#if 1
    return spline.evaluate(0.1) - spline.p0;
#else
    VectorF cubic = spline.getCubic();
    VectorF quadratic = spline.getQuadratic();
    VectorF linear = spline.getLinear();

    if(absSquared(linear) > eps * eps)
    {
        return linear;
    }

    if(absSquared(quadratic) > eps * eps)
    {
        return quadratic;
    }

    return cubic;
#endif
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
    return getPseudoAngle(getEdgeAngleVector(v));
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
    return getAngle(getEdgeAngleVector(v));
}

float getAngleDelta(MyEdge a, MyEdge b)
{
    float angle = getEdgeAngle(a) - getEdgeAngle(b);
    if(angle > M_PI)
        angle -= 2 * M_PI;
    if(angle <= -M_PI)
        angle += 2 * M_PI;
    return angle;
}

float getPolygonAngleSum(const Polygon &poly)
{
    float retval = 0;
    for(size_t i = 0; i < poly.size(); i++)
    {
        size_t j = (i + 1) % poly.size();
        size_t k = (i + 2) % poly.size();
        if(absSquared(poly[i] - poly[k]) < eps * eps)
            continue;
        float angle = getAngle(poly[j] - poly[i]) - getAngle(poly[k] - poly[j]);
        if(angle > M_PI)
            angle -= 2 * M_PI;
        if(angle <= -M_PI)
            angle += 2 * M_PI;
        retval += angle;
    }
    return retval;
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

inline ostream & operator <<(ostream & os, shared_ptr<Node> node)
{
    return os << node->position;
}

inline ostream & operator <<(ostream & os, shared_ptr<Edge> edge)
{
    return os << edge.get() << ":";
}

inline ostream & operator <<(ostream & os, const CubicSpline &spline)
{
    return os << spline.p0 << spline.dp0 << "-" << spline.p1 << spline.dp1;
}

inline ostream & operator <<(ostream & os, const Land & land)
{
    os << (land.isInverted ? "true" : "false") << "\n";
    for(size_t i = 0; i < land.edges.size(); i++)
    {
        auto edge = shared_ptr<Edge>(land.edges[i]);
        if(edge->start == land.nodes[i]) // if forward
        {
            for(CubicSpline spline : edge->cubicSplines)
            {
                os << spline << "\n";
            }
        }
        else
        {
            for(size_t j = 0; j < edge->cubicSplines.size(); j++)
            {
                os << edge->cubicSplines[edge->cubicSplines.size() - j - 1].reversed() << "\n";
            }
        }
    }
    return os;
}

inline ostream & operator <<(ostream & os, shared_ptr<Face> face)
{
    os << "Face:\n";
    for(size_t i = 0; i < face->edges.size(); i++)
    {
        auto edge = shared_ptr<Edge>(face->edges[i].edge);
        if(edge->start == face->edges[i].start) // if forward
        {
            for(CubicSpline spline : edge->cubicSplines)
            {
                os << spline << "\n";
            }
        }
        else
        {
            for(size_t j = 0; j < edge->cubicSplines.size(); j++)
            {
                os << edge->cubicSplines[edge->cubicSplines.size() - j - 1].reversed() << "\n";
            }
        }
    }
    return os;
}

inline ostream & operator <<(ostream & os, shared_ptr<Region> region)
{
    for(Land land : region->lands)
    {
        os << land;
    }
    return os;
}

inline ostream & operator <<(ostream & os, const MyEdge & edge)
{
    return os << edge.start->position << "-" << edge.end->position;
}

inline ostream & operator <<(ostream & os, unordered_multiset<MyEdge> edges)
{
    os << "Set of MyEdge:\n{\n";
    for(auto v : edges)
    {
        os << v << "\n";
    }
    return os << "}\n";
}

inline ostream & operator <<(ostream & os, vector<MyEdge> edges)
{
    os << "Vector of MyEdge:\n{\n";
    for(auto v : edges)
    {
        os << v << "\n";
    }
    return os << "}\n";
}

namespace
{
struct MyEdgeTotalEqual
{
    bool operator()(const MyEdge & a, const MyEdge & b) const
    {
        return a == b && a.edge == b.edge;
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
    assert(isValidGameState(gs, true));
    unordered_multiset<MyEdge> edges, edgesLeft;
    unordered_map<shared_ptr<Node>, vector<MyEdge>> neighborsMap;
    vector<MyEdge> path;
    vector<shared_ptr<Face>> faces;
    unordered_map<MyEdge, shared_ptr<Face>, hash<MyEdge>, MyEdgeTotalEqual> facesMap;
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

            if(absSquared(edge->cubicSplines[0].p0 - node->position) < eps * eps)
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
            edges.insert(MyEdge(edge, node, destNode));
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

    //cout << edgesLeft << flush;
    if(!edgesLeft.empty())
    {
        path.push_back(*edgesLeft.begin());
        edgesLeft.erase(edgesLeft.begin());
    }

    // find all faces
    while(!edgesLeft.empty())
    {
        //cout << path << endl;
        auto &neighbors = neighborsMap[path.back().end];
        auto curEdge = find_if(neighbors.begin(), neighbors.end(),
                                           [&path](MyEdge e) -> bool
        {
            return path.back().start == e.end && path.back().edge == e.edge;
        });
        assert(curEdge != neighbors.end());

        if(++curEdge == neighbors.end())
        {
            curEdge = neighbors.begin();
        }

        auto theEdge = *curEdge;
        if(theEdge == path.front())
        {
            //cout << "theEdge : " << theEdge << "\nEnd Of Path\n\n\n" << flush;
            shared_ptr<Face> face = make_shared<Face>();
            face->edges = std::move(path);
            path.clear();
            for(auto edge : face->edges)
            {
                assert(facesMap.find(edge) == facesMap.end());
                facesMap[edge] = face;
            }
            faces.push_back(face);
            if(!edgesLeft.empty())
            {
                path.push_back(*edgesLeft.begin());
                edgesLeft.erase(edgesLeft.begin());
            }
        }
        else
        {
            //cout << "theEdge : " << theEdge << "\n\n\n\n" << flush;
            path.push_back(theEdge);
            auto range = edgesLeft.equal_range(theEdge);
            auto i = get<0>(range);
            while(i != get<1>(range))
            {
                if(i->edge == theEdge.edge)
                    break;
                else
                    i++;
            }
            assert(i != get<1>(range));
            edgesLeft.erase(i);
        }
        //cout << edgesLeft << flush;
    }

    if(!path.empty())
    {
        shared_ptr<Face> face = make_shared<Face>();
        face->edges = std::move(path);
        for(auto edge : face->edges)
        {
            assert(facesMap.find(edge) == facesMap.end());
            facesMap[edge] = face;
        }
        faces.push_back(face);
    }

    // compute face attributes

    for(shared_ptr<Face> face : faces)
    {
        face->polygon = getFacePolygon(face);
        float angleSum = getPolygonAngleSum(face->polygon);
        face->isOutside = false;
        if(angleSum <= 0)
            face->isOutside = true;
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
                if(interiorFace->isOutside)
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
        if(!face->parent.lock() && face->isOutside)
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
    //cout << "Faces:\n";
//    for(shared_ptr<Face> face : faces)
//    {
//        cout << face << "\n";
//    }
//    cout << "Regions:\n";
//    for(shared_ptr<Region> region : regions)
//    {
//        cout << region << "\n";
//    }
//    cout << "Graph:\n" << *gs << endl;
}

bool isPointInPolygon(const Polygon & poly, VectorF p)
{
    assert(poly.size() >= 3);
    p += VectorF(eps);
    size_t hitCount = 0;
    //cout << p << " ";
    for(size_t i = 0; i < poly.size(); i++)
    {
        size_t j = (i + 1) % poly.size();
        VectorF delta = poly[j] - poly[i];
        assert(absSquared(delta) > eps * eps);
        if(abs(delta.y) < eps)
            continue;
        float t = (p.y - poly[i].y) / delta.y;
        if(t <= eps || t > 1 + eps)
            continue;
        VectorF hitPt = poly[i] + t * delta;
        if(hitPt.x < p.x)
            continue;
        hitCount++;
        //cout << poly[i] << "-" << poly[j] << hitPt << " ";
    }
    //cout << ": " << hitCount << "\n";
    return (hitCount % 2 != 0);
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
    assert(r);
    for(Land land : r->lands)
    {
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
    return true;
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

GameState duplicate(GameState gs, vector<shared_ptr<Node> *> translateList)
{
    GameState retval = makeEmptyGameState();
    unordered_map<shared_ptr<Node>, shared_ptr<Node>> nodeMap;
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
    for(shared_ptr<Node> * i : translateList)
    {
        assert(i);
        shared_ptr<Node> & node = *i;
        node = nodeMap[node];
    }
    return retval;
}

shared_ptr<Region> pointToRegion(GameState gs, VectorF p)
{
    //cout << "\x1b[H\x1b[2J";
    unordered_set<shared_ptr<Region>> regions;
    for(auto ni = gs->begin(); ni != gs->end(); ni++)
    {
        for(auto ei = gs->begin(ni); ei != gs->end(ni); ei++)
        {
            shared_ptr<Edge> edge = get<0>(*ei);
            regions.insert(edge->inside);
            regions.insert(edge->outside);
        }
    }
    for(shared_ptr<Region> region : regions)
    {
        if(isPointInRegion(region, p))
            return region;
    }
    return nullptr;
}

GameState move(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode, const vector<CubicSpline> & path)
{
    assert(gs);
    assert(startNode);
    assert(endNode);
    assert(path.size() >= 1);
    if(isPathSelfIntersecting(path))
        return nullptr;
    assert(absSquared(path.front().p0 - startNode->position) < eps * eps);
    assert(absSquared(path.back().p1 - endNode->position) < eps * eps);
    for(size_t i = 1; i < path.size(); i++)
    {
        assert(absSquared(path[i - 1].p1 - path[i].p0) < eps * eps);
    }
    VectorF pathCenterPosition = path[path.size() / 2].evaluate(0.5f * (path.size() % 2));
    gs = duplicate(gs, vector<shared_ptr<Node> *>{&startNode, &endNode});
    auto startNodeIterator = find(gs->begin(), gs->end(), startNode);
    assert(startNodeIterator != gs->end());
    auto endNodeIterator = find(gs->begin(), gs->end(), endNode);
    assert(endNodeIterator != gs->end());
    auto newNodeIterator = gs->addNode(make_shared<Node>(pathCenterPosition));
    vector<CubicSpline> path1, path2;
    path1.reserve((path.size() + 1) / 2);
    path2.reserve((path.size() + 1) / 2);
    for(size_t i = 0; i < path.size() / 2; i++)
    {
        path1.push_back(path[i]);
    }
    if(path.size() % 2 != 0)
    {
        path1.push_back(path[path.size() / 2].subSection(0, 0.5f));
        path1.back().p1 = pathCenterPosition;
        path2.push_back(path[path.size() / 2].subSection(0.5f, 1));
        path2.front().p0 = pathCenterPosition;
    }
    for(size_t i = (path.size() + 1) / 2; i < path.size(); i++)
    {
        path2.push_back(path[i]);
    }
    shared_ptr<Edge> edge1 = shared_ptr<Edge>(new Edge(path1, startNode, *newNodeIterator));
    shared_ptr<Edge> edge2 = shared_ptr<Edge>(new Edge(path2, *newNodeIterator, endNode));
    gs->addEdge(edge1, startNodeIterator, newNodeIterator);
    gs->addEdge(edge1, newNodeIterator, startNodeIterator);
    gs->addEdge(edge2, newNodeIterator, endNodeIterator);
    gs->addEdge(edge2, endNodeIterator, newNodeIterator);
    if(isValidGameState(gs))
    {
        recalculateRegions(gs);
        return gs;
    }
    return nullptr;
}

Edge::Edge(const vector<CubicSpline> & cubicSplines, shared_ptr<Node> start, shared_ptr<Node> end)
    : cubicSplines(cubicSplines)
{
    this->start = start;
    this->end = end;
//    assert(cubicSplines[0].p0 == start->position);
//    assert(cubicSplines.back().p1 == end->position);
}
