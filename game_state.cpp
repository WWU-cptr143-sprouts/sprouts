#include "game_state.h"
#include "texture_atlas.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

/**************
 *Description:
 *Input:
 *Output:
**************/

namespace
{
initializer init1([]()
{
    srand((unsigned)time(nullptr));
});
}

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

            for(CubicSpline spline1 : edge1)
            {
                for(CubicSpline spline2 : edge2)
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

bool doesCubicSplineListIntersect(GameState gs, vector<CubicSpline> splines)
{
    unordered_set<shared_ptr<Edge>> edgesSet;
    vector<shared_ptr<Edge>> edges;

    for(auto nodeIterator = gs->begin(); nodeIterator != gs->end(); nodeIterator++)
    {
        for(auto edgeIterator = gs->begin(nodeIterator); edgeIterator != gs->end(nodeIterator);
                edgeIterator++)
        {
            shared_ptr<Edge> edge = get<0>(*edgeIterator);

            if(get<1>(edgesSet.insert(edge)))
            {
                edges.push_back(edge);
            }
        }
    }

    vector<CubicSpline> edge1 = splines;
    const float adjustDistance = 0.1;
    edge1[0] = edge1[0].subSection(adjustDistance, 1);
    edge1[edge1.size() - 1] = edge1[edge1.size() - 1].subSection(0, 1 - adjustDistance);

    for(auto edge : edges)
    {
        const vector<CubicSpline> &edge2 = edge->cubicSplines;

        for(CubicSpline spline1 : edge1)
        {
            for(CubicSpline spline2 : edge2)
            {
                if(spline1.intersects(spline2))
                {
                    return true;
                }
            }
        }
    }

    return false;
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
        : edge(edge), start(reversed ? edge->end : edge->start), end(reversed ? edge->start : edge->end),
          face(shared_ptr<Face>(nullptr))
    {
    }
    MyEdge(shared_ptr<Edge> edge, shared_ptr<Node> start, shared_ptr<Node> end,
           shared_ptr<Face> face = nullptr)
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

float getPseudoAngle(VectorF
                     v) // doesn't really return the angle : returns a number that sorts in the same order but can be calculated faster
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
    {
        angle -= 2 * M_PI;
    }

    if(angle < 0)
    {
        angle += 2 * M_PI;
    }

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
    {
        angle -= 2 * M_PI;
    }

    if(angle <= -M_PI)
    {
        angle += 2 * M_PI;
    }

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
        {
            retval -= M_PI;
            continue;
        }

        float angle = getAngle(poly[j] - poly[i]) - getAngle(poly[k] - poly[j]);

        if(angle > M_PI)
        {
            angle -= 2 * M_PI;
        }

        if(angle <= -M_PI)
        {
            angle += 2 * M_PI;
        }

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

inline ostream &operator <<(ostream &os, shared_ptr<Node> node)
{
    return os << node->position;
}

inline ostream &operator <<(ostream &os, shared_ptr<Edge> edge)
{
    return os << edge.get() << ":";
}

inline ostream &operator <<(ostream &os, const CubicSpline &spline)
{
    return os << spline.p0 << spline.dp0 << "-" << spline.p1 << spline.dp1;
}

inline ostream &operator <<(ostream &os, const Land &land)
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

inline ostream &operator <<(ostream &os, shared_ptr<Face> face)
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

inline ostream &operator <<(ostream &os, shared_ptr<Region> region)
{
    for(Land land : region->lands)
    {
        os << land;
    }

    return os;
}

inline ostream &operator <<(ostream &os, const MyEdge &edge)
{
    return os << edge.start->position << "-" << edge.end->position;
}

inline ostream &operator <<(ostream &os, unordered_multiset<MyEdge> edges)
{
    os << "Set of MyEdge:\n{\n";

    for(auto v : edges)
    {
        os << v << "\n";
    }

    return os << "}\n";
}

inline ostream &operator <<(ostream &os, vector<MyEdge> edges)
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
    bool operator()(const MyEdge &a, const MyEdge &b) const
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
        {
            isolatedNodes.push_back(node);
        }
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
                {
                    break;
                }
                else
                {
                    i++;
                }
            }

            assert(i != get<1>(range));
            edgesLeft.erase(i);
        }

        //cout << edgesLeft << flush;
    }

    unordered_map<shared_ptr<Node>, shared_ptr<Face>> nodeFacesMap;

    if(!path.empty())
    {
        shared_ptr<Face> face = make_shared<Face>();
        face->edges = std::move(path);

        for(auto edge : face->edges)
        {
            assert(facesMap.find(edge) == facesMap.end());
            facesMap[edge] = face;
            nodeFacesMap[edge.start] = face;
            nodeFacesMap[edge.end] = face;
        }

        faces.push_back(face);
    }

    // calculate graph partitions
    unordered_map<shared_ptr<Node>, shared_ptr<unordered_set<shared_ptr<Node>>>> partitionsMap;

    for(shared_ptr<Node> node : *gs)
    {
        partitionsMap[node] = make_shared<unordered_set<shared_ptr<Node>>>(unordered_set<shared_ptr<Node>> {node});
    }

    unordered_set<shared_ptr<Node>> nodeUsedInPartition;

    for(shared_ptr<Node> node : *gs)
    {
        if(nodeUsedInPartition.find(node) != nodeUsedInPartition.end())
        {
            continue;
        }

        shared_ptr<unordered_set<shared_ptr<Node>>> partition =
            make_shared<unordered_set<shared_ptr<Node>>>(unordered_set<shared_ptr<Node>> {node});
        bool anyChange = true;

        while(anyChange)
        {
            anyChange = false;
            vector<shared_ptr<Node>> partitionNodes;
            partitionNodes.assign(partition->begin(), partition->end());

            for(shared_ptr<Node> nodeInPartition : partitionNodes)
            {
                for(MyEdge edge : neighborsMap[nodeInPartition])
                {
                    if(get<1>(partition->insert(edge.end)))
                    {
                        anyChange = true;
                    }
                }
            }
        }

        for(shared_ptr<Node> nodeInPartition : *partition)
        {
            partitionsMap[nodeInPartition] = partition;
            nodeUsedInPartition.insert(nodeInPartition);
        }
    }

    unordered_map<shared_ptr<unordered_set<shared_ptr<Node>>>, shared_ptr<DisjointPartition>>
            partitions;

    for(shared_ptr<Node> node : *gs)
    {
        shared_ptr<unordered_set<shared_ptr<Node>>> partition = partitionsMap[node];
        shared_ptr<DisjointPartition> &finalPartition = partitions[partition];

        if(finalPartition == nullptr)
        {
            finalPartition = make_shared<DisjointPartition>();
        }

        finalPartition->nodes.push_back(node);
        node->partition = finalPartition;
    }

    // compute face attributes

    for(shared_ptr<Face> face : faces)
    {
        face->polygon = getFacePolygon(face);
        float angleSum = getPolygonAngleSum(face->polygon);
        face->isOutside = false;

        if(angleSum <= eps)
        {
            face->isOutside = true;
        }
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
                {
                    face->interiorFaces.insert(interiorFace);
                }
            }
        }
        else
        {
            for(MyEdge edge : edges)
            {
                bool canTest = true;

                for(MyEdge edge2 : face->edges)
                {
                    if(edge.start == edge2.start || edge.start == edge2.end || edge.end == edge2.start
                            || edge.end == edge2.end)
                    {
                        canTest = false;
                        break;
                    }
                }

                if(!canTest)
                {
                    continue;
                }

                if(!isPointInPolygon(face->polygon, edge.start->position))
                {
                    continue;
                }

                shared_ptr<Face> interiorFace = facesMap[edge];

                if(interiorFace->isOutside)
                {
                    face->interiorFaces.insert(interiorFace);
                }
            }
        }
    }

    // convert to a tree and create lands

    for(shared_ptr<Face> face : faces)
    {
        face->removeList.reserve(faces.size());

        for(shared_ptr<Face> interiorFace : face->interiorFaces)
        {
            copy(interiorFace->interiorFaces.begin(), interiorFace->interiorFaces.end(),
                 back_inserter(face->removeList));
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

    // finish setting parents

    for(shared_ptr<Face> face : faces)
    {
        if(face->isOutside)
        {
            continue;
        }

        if(!face->parent.lock())
        {
            unordered_set<shared_ptr<Face>> facesSet;
            facesSet.insert(face);
            bool didAnything = true;

            while(didAnything && !face->parent.lock())
            {
                didAnything = false;

                for(MyEdge edge : face->edges)
                {
                    shared_ptr<Face> otherFace = facesMap[edge.reversed()];

                    if(otherFace->isOutside)
                    {
                        face->parent = otherFace;
                        break;
                    }
                    else if(otherFace->parent.lock())
                    {
                        face->parent = otherFace->parent;
                        break;
                    }

                    if(get<1>(facesSet.insert(otherFace)))
                    {
                        didAnything = true;
                    }
                }
            }

            assert(face->parent.lock());
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
                node->partition->containingRegion = region;
                break;
            }
        }
    }

    // add faces to partitions

    for(shared_ptr<Face> face : faces)
    {
        if(face->isOutside)
        {
            for(MyEdge edge : face->edges)
            {
                edge.start->partition->containingRegion = face->region;
                edge.end->partition->containingRegion = face->region;
            }
        }
        else
        {
            shared_ptr<Face> parentFace = face->parent.lock();
            assert(parentFace);

            for(MyEdge edge : face->edges)
            {
                edge.start->partition->containingRegion = parentFace->region;
                edge.end->partition->containingRegion = parentFace->region;
            }
        }
    }
}

bool isPointInPolygon(const Polygon &poly, VectorF p)
{
    assert(poly.size() >= 3);
    p += VectorF(eps);
    size_t hitCount = 0;

    //cout << p << " ";
    for(size_t i = 0; i < poly.size(); i++)
    {
        size_t j = (i + 1) % poly.size();
        VectorF delta = poly[j] - poly[i];

        if(abs(delta.y) < eps)
        {
            continue;
        }

        float t = (p.y - poly[i].y) / delta.y;

        if(t <= eps || t > 1 + eps)
        {
            continue;
        }

        VectorF hitPt = poly[i] + t * delta;

        if(hitPt.x < p.x)
        {
            continue;
        }

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
            {
                return false;
            }
        }
        else if(!land.isInverted)
        {
            return false;
        }
    }

    return true;
}

#if 0
#warning finish implementing splitPolygon
#else
vector<Polygon> splitPolygon(Polygon poly)
{
    assert(isNonComplex(poly));
    vector<Polygon> retval;

    while(poly.size() > 3)
    {
        retval.push_back(Polygon());
        size_t polyIndex = 0;
        retval.back().push_back(poly[polyIndex++]);
        retval.back().push_back(poly[polyIndex++]);
        retval.back().push_back(poly[polyIndex++]); // push 3 points for a triangle

        for(;;)
        {
            retval.back().push_back(poly[polyIndex]);

            if(!isConvexPolygon(retval.back()))
            {
                retval.back().pop_back();
                break;
            }

            if(polyIndex != poly.size() - 1)
            {
                bool intersects = false;

                for(size_t i = polyIndex; i < poly.size(); i++)
                {
                    size_t j = (i + 1) % poly.size();

                    if(linesIntersect(poly[0], poly[polyIndex], poly[i], poly[j]))
                    {
                        intersects = true;
                        break;
                    }
                }

                if(intersects)
                {
                    retval.back().pop_back();
                    break;
                }
            }

            polyIndex++;
        }

        poly.erase(poly.begin() + 1, poly.begin() + (polyIndex - 1));
    }

    if(poly.size() >= 3)
    {
        retval.push_back(poly);
    }

    return retval;
}
#endif

GameState duplicate(GameState gs, vector<shared_ptr<Node> *> translateNodeList,
                    vector<shared_ptr<Edge> *> translateEdgeList)
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
        edgeMap[edge] = newEdge;
    }

    recalculateRegions(retval);

    for(shared_ptr<Node> *i : translateNodeList)
    {
        assert(i);
        shared_ptr<Node> &node = *i;
        node = nodeMap[node];
    }

    for(shared_ptr<Edge> *i : translateEdgeList)
    {
        assert(i);
        shared_ptr<Edge> &edge = *i;
        edge = edgeMap[edge];
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
        {
            return region;
        }
    }

    return nullptr;
}

namespace
{
vector<CubicSpline> makeCircularPath(VectorF position, VectorF dir, float size)
{
    dir = normalize(dir);
    VectorF sideways = normalize(cross(dir, VectorF(0, 0, 1))) * 2;
    return vector<CubicSpline> {CubicSpline(position, position + dir * size, sideways * size, -sideways * size), CubicSpline(position + dir * size, position, -sideways * size, sideways * size)};
}
}

GameStateMove::GameStateMove(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode,
                             vector<CubicSpline> path, bool isNestedCall)
{
    assert(gs);
    this->initialState = gs;
    assert(startNode);
    this->startNode = startNode;
    assert(endNode);
    this->endNode = endNode;
    assert(path.size() >= 1);

#if 0
    if(!isNestedCall)
    {
        if(gs->edgeCount() == 0 && startNode != endNode)
            path = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};
        else if(startNode == endNode)
        {
            shared_ptr<Edge> edge = nullptr;
            bool isEdgeStart = false;

            for(auto ei = gs->begin(startNode->iter); ei != gs->end(startNode->iter); ei++)
            {
                if(edge != nullptr)
                {
                    throw InvalidMoveException();
                }

                edge = get<0>(*ei);

                if(edge->start == startNode)
                {
                    isEdgeStart = true;
                }

                if(edge->end == startNode && isEdgeStart)
                {
                    throw InvalidMoveException();
                }
            }

            VectorF nodeDir = normalizeNoThrow(startNode->position);

            if(absSquared(nodeDir) < eps * eps)
            {
                nodeDir = VectorF(1, 0, 0);
            }

            if(edge)
            {
                CubicSpline spline = (isEdgeStart ? edge->cubicSplines.front() :
                                      edge->cubicSplines.back().reversed());

                if(absSquared(spline.getLinear()) > eps * eps)
                {
                    nodeDir = -normalize(spline.getLinear());
                }
                else if(absSquared(spline.getQuadratic()) > eps * eps)
                {
                    nodeDir = -normalize(spline.getQuadratic());
                }
                else
                {
                    nodeDir = -normalize(spline.getCubic());
                }
            }

            bool isValid = true;
            Polygon poly = getSplineLoopPoints(path);
            GameStateMove gsm(gs, startNode, endNode, path, true);

            for(auto partition : gsm.insidePartitions)
            {
                for(auto wnode : partition->nodes)
                {
                    auto node = shared_ptr<Node>(wnode);

                    if(isPointInPolygon(poly, node->position))
                    {
                        isValid = false;
                        break;
                    }
                }

                if(!isValid)
                {
                    break;
                }
            }

            for(auto partition : gsm.outsidePartitions)
            {
                for(auto wnode : partition->nodes)
                {
                    auto node = shared_ptr<Node>(wnode);

                    if(isPointInPolygon(poly, node->position))
                    {
                        isValid = false;
                        break;
                    }
                }

                if(!isValid)
                {
                    break;
                }
            }

            float minSize = 0.01;
            float maxSize = 0.3;

            if(isValid
                    && !doesCubicSplineListIntersect(gs, makeCircularPath(startNode->position, nodeDir, minSize)))
            {
                for(int i = 0; i < 6; i++)
                {
                    float testSize = (minSize + maxSize) * 0.5f;

                    if(doesCubicSplineListIntersect(gs, makeCircularPath(startNode->position, nodeDir, testSize)))
                    {
                        maxSize = testSize;
                    }
                    else
                    {
                        minSize = testSize;
                    }
                }

                float size = max(minSize - 0.04f, minSize * 0.5f);
                path = makeCircularPath(startNode->position, nodeDir, size);
            }
        }
        else if(startNode != endNode && getRegions(gs).size() == 1)
        {
            auto newPath = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};

            if(!doesCubicSplineListIntersect(gs, newPath))
            {
                path = newPath;
            }
        }
    }
#endif

    if(isPathSelfIntersecting(path))
    {
        throw InvalidMoveException();
    }

    assert(absSquared(path.front().p0 - startNode->position) < eps * eps);
    assert(absSquared(path.back().p1 - endNode->position) < eps * eps);

    for(size_t i = 1; i < path.size(); i++)
    {
        assert(absSquared(path[i - 1].p1 - path[i].p0) < eps * eps);
    }

    VectorF pathCenterPosition = path[path.size() / 2].evaluate(0.5f * (path.size() % 2));
    containingRegion = pointToRegion(gs, pathCenterPosition);
    unordered_map<shared_ptr<Node>, shared_ptr<Node>> nodeMap, inverseNodeMap;
    vector<shared_ptr<Node> *> nodePtrs;

    for(auto node : *gs)
    {
        nodeMap[node] = node;
        nodePtrs.push_back(&nodeMap[node]);
    }

    nodePtrs.push_back(&startNode);
    nodePtrs.push_back(&endNode);
    gs = duplicate(gs, std::move(nodePtrs));
    nodePtrs.clear();

    for(auto v : nodeMap)
    {
        inverseNodeMap[get<1>(v)] = get<0>(v);
    }

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
        finalState = gs;
        auto partitions = getPartitions(gs);

        if(startNode == endNode)
        {
            Polygon poly = getSplineLoopPoints(path);

            for(auto partition : partitions)
            {
                if(partition->containingRegion.lock() == edge1->inside
                        || partition->containingRegion.lock() == edge1->outside)
                {
                    if(isPointInPolygon(poly, partition->nodes.front().lock()->position))
                    {
                        insidePartitions.push_back(inverseNodeMap[partition->nodes.front().lock()]->partition);
                    }
                    else
                    {
                        outsidePartitions.push_back(inverseNodeMap[partition->nodes.front().lock()]->partition);
                    }
                }
            }
        }
        else
        {
            for(auto partition : partitions)
            {
                if(partition->containingRegion.lock() == edge1->inside)
                {
                    insidePartitions.push_back(inverseNodeMap[partition->nodes.front().lock()]->partition);
                }
                else if(partition->containingRegion.lock() == edge1->outside)
                {
                    outsidePartitions.push_back(inverseNodeMap[partition->nodes.front().lock()]->partition);
                }
            }
        }
    }
    else
    {
        throw InvalidMoveException();
    }
}

GameStateMove::GameStateMove(GameState gs, shared_ptr<Node> startNode, shared_ptr<Node> endNode,
                             vector<shared_ptr<DisjointPartition>> insidePartitions,
                             vector<shared_ptr<DisjointPartition>> outsidePartitions, shared_ptr<Region> containingRegion)
{
    this->initialState = gs;
    this->startNode = startNode;
    this->endNode = endNode;
    this->insidePartitions = insidePartitions;
    this->outsidePartitions = outsidePartitions;
    this->containingRegion = containingRegion;
}

namespace
{
typedef tuple<VectorF, shared_ptr<void>, bool> MyNode;
typedef array<MyNode, 3> MyTriangle;
bool pointInTriangle(const MyTriangle &tri, VectorF p)
{
    p += VectorF(eps);
    size_t hitCount = 0;

    for(size_t i = 0; i < tri.size(); i++)
    {
        size_t j = (i + 1) % tri.size();
        VectorF delta = get<0>(tri[j]) - get<0>(tri[i]);

        if(abs(delta.y) < eps)
        {
            continue;
        }

        float t = (p.y - get<0>(tri[i]).y) / delta.y;

        if(t <= eps || t > 1 + eps)
        {
            continue;
        }

        VectorF hitPt = get<0>(tri[i]) + t * delta;

        if(hitPt.x < p.x)
        {
            continue;
        }

        hitCount++;
    }

    return (hitCount % 2 != 0);
}

MyTriangle getEnclosingTriangle(const vector<pair<VectorF, shared_ptr<void>>> &points,
                                float offset = 0.1)
{
    assert(points.size() >= 3);
    const VectorF edge1Normal = normalize(VectorF(0, -1, 0));
    const VectorF edge2Normal = normalize(VectorF(-1, 1, 0));
    const VectorF edge3Normal = normalize(VectorF(1, 1, 0));
    const VectorF triangleNormal = VectorF(0, 0, 1);
    float edge1Min = 0;
    float edge2Min = 0;
    float edge3Min = 0;
    bool first = true;

    for(pair<VectorF, shared_ptr<void>> node : points)
    {
        float edge1Value = dot(get<0>(node), edge1Normal);
        float edge2Value = dot(get<0>(node), edge2Normal);
        float edge3Value = dot(get<0>(node), edge3Normal);

        if(first || edge1Min > edge1Value)
        {
            edge1Min = edge1Value;
        }

        if(first || edge2Min > edge2Value)
        {
            edge2Min = edge2Value;
        }

        if(first || edge3Min > edge3Value)
        {
            edge3Min = edge3Value;
        }

        first = false;
    }

    edge1Min -= offset;
    edge2Min -= offset;
    edge3Min -= offset;
    MyTriangle retval;
    retval[0] = make_tuple(findTriplePlaneIntersection(edge1Normal, -edge1Min, edge2Normal, -edge2Min,
                           triangleNormal, 0), shared_ptr<Node>(nullptr), false);
    retval[1] = make_tuple(findTriplePlaneIntersection(edge2Normal, -edge2Min, edge3Normal, -edge3Min,
                           triangleNormal, 0), shared_ptr<Node>(nullptr), false);
    retval[2] = make_tuple(findTriplePlaneIntersection(edge3Normal, -edge3Min, edge1Normal, -edge1Min,
                           triangleNormal, 0), shared_ptr<Node>(nullptr), false);
    return std::move(retval);
}

bool isPointInCircumcircle(VectorF p1, VectorF p2, VectorF p3, VectorF testPoint)
{
    if(absSquared(cross(p1 - p2, p3 - p2)) < eps * eps * eps * eps)
    {
        return false;
    }

    //       | testPoint.x * testPoint.x + testPoint.y * testPoint.y  testPoint.x  testPoint.y  1 |
    // det = | p1.x * p1.x + p1.y * p1.y                              p1.x         p1.y         1 |
    //       | p2.x * p2.x + p2.y * p2.y                              p2.x         p2.y         1 |
    //       | p3.x * p3.x + p3.y * p3.y                              p3.x         p3.y         1 |
    //
    // see http://mathworld.wolfram.com/Circumcircle.html  equation 2
    float det = p2.x * p1.y * (-(testPoint.x * testPoint.x + testPoint.y * testPoint.y)) + p3.x * p1.y
                * (testPoint.x * testPoint.x + testPoint.y * testPoint.y) + testPoint.x * p1.y *
                (p2.x * p2.x + p2.y * p2.y) - p3.x * p1.y * (p2.x * p2.x + p2.y * p2.y) - testPoint.x * p1.y *
                (p3.x * p3.x + p3.y * p3.y) + p2.x * p1.y * (p3.x * p3.x + p3.y * p3.y) + p1.x * p2.y *
                (testPoint.x * testPoint.x + testPoint.y * testPoint.y) - p3.x * p2.y *
                (testPoint.x * testPoint.x + testPoint.y * testPoint.y) - p1.x * p3.y *
                (testPoint.x * testPoint.x + testPoint.y * testPoint.y) + p2.x * p3.y *
                (testPoint.x * testPoint.x + testPoint.y * testPoint.y) + p2.x * testPoint.y *
                (p1.x * p1.x + p1.y * p1.y) - p3.x * testPoint.y * (p1.x * p1.x + p1.y * p1.y) - testPoint.x * p2.y
                * (p1.x * p1.x + p1.y * p1.y) + p3.x * p2.y * (p1.x * p1.x + p1.y * p1.y) + testPoint.x * p3.y *
                (p1.x * p1.x + p1.y * p1.y) - p2.x * p3.y * (p1.x * p1.x + p1.y * p1.y) - p1.x * testPoint.y *
                (p2.x * p2.x + p2.y * p2.y) + p3.x * testPoint.y * (p2.x * p2.x + p2.y * p2.y) - testPoint.x * p3.y
                * (p2.x * p2.x + p2.y * p2.y) + p1.x * p3.y * (p2.x * p2.x + p2.y * p2.y) + p1.x * testPoint.y *
                (p3.x * p3.x + p3.y * p3.y) - p2.x * testPoint.y * (p3.x * p3.x + p3.y * p3.y) + testPoint.x * p2.y
                * (p3.x * p3.x + p3.y * p3.y) - p1.x * p2.y * (p3.x * p3.x + p3.y * p3.y);
    return det * sgn(cross(p1 - p2, p3 - p2).z) >= 0;
}

inline ostream &operator <<(ostream &os, MyTriangle tri)
{
    return os << "[" << get<0>(tri[0]) << ", " << get<0>(tri[1]) << ", " << get<0>(tri[2]) << "]";
}

vector<MyTriangle> delaunayTriangulation(const vector<pair<VectorF, shared_ptr<void>>> &points,
        size_t stopStep = -1) // stopStep is for debugging
{
    if(points.size() < 3)
    {
        return vector<MyTriangle>();
    }

    vector<MyTriangle> retval;
    retval.push_back(getEnclosingTriangle(points));
    //    cout << "\x1b[H";
    //    cout << "inserted : " << retval.back() << "\x1b[K\n";
    size_t step = 0;

    for(pair<VectorF, shared_ptr<void>> node : points)
    {
        //        cout << "processing point : " << get<0>(node) << "\x1b[K\n";
        vector<MyTriangle> intersectingTriangles;

        for(auto triangleIterator = retval.begin(); triangleIterator != retval.end();)
        {
            const MyTriangle &triangle = *triangleIterator;

            if(isPointInCircumcircle(get<0>(triangle[0]), get<0>(triangle[1]), get<0>(triangle[2]),
                                     get<0>(node)))
            {
                //                cout << "removed : " << triangle << "\x1b[K\n";
                intersectingTriangles.push_back(std::move(triangle));
                triangleIterator = retval.erase(triangleIterator);
            }
            else
            {
                triangleIterator++;
            }
        }

        if(step++ >= stopStep)
        {
            return retval;
        }

        vector<MyNode> edgePoints;

        for(const MyTriangle &triangle : intersectingTriangles)
        {
            for(MyNode point : triangle)
            {
                if(none_of(edgePoints.begin(), edgePoints.end(), [&point](const MyNode & currentPoint)->bool
            {
                return get<0>(point) == get<0>(currentPoint);
                }))
                {
                    edgePoints.push_back(point);
                }
            }
        }

        sort(edgePoints.begin(), edgePoints.end(), [&node](const MyNode & a, const MyNode & b)
        {
            return getPseudoAngle(get<0>(a) - get<0>(node)) < getPseudoAngle(get<0>(b) - get<0>(node));
        });

        for(size_t i = 0; i < edgePoints.size(); i++)
        {
            size_t j = (i + 1) % edgePoints.size();
            MyTriangle triangle;
            triangle[0] = edgePoints[i];
            triangle[1] = edgePoints[j];
            triangle[2] = make_tuple(get<0>(node), get<1>(node), true);
            retval.push_back(std::move(triangle));
            //            cout << "inserted : " << retval.back() << "\x1b[K\n";
        }

        if(step++ >= stopStep)
        {
            return retval;
        }
    }

    for(auto triangleIterator = retval.begin(); triangleIterator != retval.end();)
    {
        const MyTriangle &triangle = *triangleIterator;

        if(!get<2>(triangle[0]) || !get<2>(triangle[1]) || !get<2>(triangle[2]))
        {
            triangleIterator = retval.erase(triangleIterator);
        }
        else
        {
            triangleIterator++;
        }
    }

    return retval;
}
}

vector<vector<pair<VectorF, shared_ptr<void>>>> getDelaunayTriangulation(
    const vector<pair<VectorF, shared_ptr<void>>> &points, size_t stopStep)
{
    vector<MyTriangle> triangulation = delaunayTriangulation(points, stopStep);
    vector<vector<pair<VectorF, shared_ptr<void>>>> retval;
    retval.reserve(triangulation.size());

    for(const MyTriangle &triangle : triangulation)
    {
        vector<pair<VectorF, shared_ptr<void>>> newTriangle;
        newTriangle.reserve(triangle.size());

        for(const MyNode point : triangle)
        {
            newTriangle.push_back(make_pair(get<0>(point), get<1>(point)));
        }

        retval.push_back(std::move(newTriangle));
    }

    return std::move(retval);
}

namespace
{
template <typename T>
bool doesSetsMatch(const vector<shared_ptr<T>> &a, const vector<shared_ptr<T>> &b)
{
    unordered_set<shared_ptr<T>> aSet, bSet;
    for(shared_ptr<T> v : a)
    {
        aSet.insert(v);
    }
    for(shared_ptr<T> v : b)
    {
        bSet.insert(v);
    }
    for(shared_ptr<T> v : a)
    {
        if(bSet.find(v) == bSet.end())
            return false;
    }
    for(shared_ptr<T> v : b)
    {
        if(aSet.find(v) == aSet.end())
            return false;
    }
    return true;
}
}

bool GameStateMove::doesPathWork(const vector<CubicSpline> &path) const
{
    if(doesCubicSplineListIntersect(initialState, path))
        return false;
    try
    {
        GameStateMove gsm(initialState, startNode, endNode, path, true);
        if(gsm.containingRegion != containingRegion)
            return false;
        if(!doesSetsMatch(insidePartitions, gsm.insidePartitions))
            return false;
        if(!doesSetsMatch(outsidePartitions, gsm.outsidePartitions))
            return false;
    }
    catch(InvalidMoveException & e)
    {
        return false;
    }
    return true;
}

void GameStateMove::calculateFinalState()
{
    vector<CubicSpline> path;
    GameState gs = initialState;
    unordered_map<shared_ptr<Node>, vector<shared_ptr<Node>>> neighborMap;
    auto edges = getEdges(gs);

    for(auto edge : edges)
    {
        neighborMap[edge->start].push_back(edge->end);
        neighborMap[edge->end].push_back(edge->start);
    }

    if(gs->edgeCount() == 0 && startNode != endNode)
        path = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};
    else if(startNode == endNode && insidePartitions.empty())
    {
        shared_ptr<Edge> edge = nullptr;
        bool isEdgeStart = false;

        for(auto ei = gs->begin(startNode->iter); ei != gs->end(startNode->iter); ei++)
        {
            assert(edge == nullptr);
            edge = get<0>(*ei);

            if(edge->start == startNode)
            {
                isEdgeStart = true;
                assert(edge->end != startNode);
            }
        }

        VectorF nodeDir = normalizeNoThrow(startNode->position);

        if(absSquared(nodeDir) < eps * eps)
        {
            nodeDir = VectorF(1, 0, 0);
        }

        if(edge)
        {
            CubicSpline spline = (isEdgeStart ? edge->cubicSplines.front() :
                                  edge->cubicSplines.back().reversed());

            if(absSquared(spline.getLinear()) > eps * eps)
            {
                nodeDir = -normalize(spline.getLinear());
            }
            else if(absSquared(spline.getQuadratic()) > eps * eps)
            {
                nodeDir = -normalize(spline.getQuadratic());
            }
            else
            {
                nodeDir = -normalize(spline.getCubic());
            }
        }

        float minSize = 0.01;
        float maxSize = 0.3;

        if(!doesCubicSplineListIntersect(gs, makeCircularPath(startNode->position, nodeDir, minSize)))
        {
            for(int i = 0; i < 6; i++)
            {
                float testSize = (minSize + maxSize) * 0.5f;

                if(doesCubicSplineListIntersect(gs, makeCircularPath(startNode->position, nodeDir, testSize)))
                {
                    maxSize = testSize;
                }
                else
                {
                    minSize = testSize;
                }
            }

            float size = max(minSize - 0.04f, minSize * 0.5f);
            path = makeCircularPath(startNode->position, nodeDir, size);
        }
    }
    else if(startNode != endNode && getRegions(gs).size() == 1)
    {
        auto newPath = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};

        if(!doesCubicSplineListIntersect(gs, newPath))
        {
            path = newPath;
        }
    }

    if(startNode != endNode && path.empty())
    {
        bool anyInside = false;
        shared_ptr<Region> theRegion = pointToRegion(gs, 0.5f * (startNode->position + endNode->position));

        if(theRegion == containingRegion && !doesCubicSplineListIntersect(gs, vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)}))
        {
            if(doesPathWork(vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)}))
                path = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};
        }
    }

//    if(startNode != endNode && path.empty())
//    {
//        if(neighborMap[startNode].size() == 2 && neighborMap[startNode][0] == endNode
//                && neighborMap[startNode][1] == endNode &&
//                neighborMap[endNode].size() == 2 && neighborMap[endNode][0] == startNode
//                && neighborMap[endNode][1] == startNode)
//        {
//            bool anyInside = false;
//            shared_ptr<Region> theRegion = pointToRegion(gs, 0.5f * (startNode->position + endNode->position));
//
//            if(theRegion == containingRegion && !doesCubicSplineListIntersect(gs, vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)}))
//            {
//                for(auto partition : insidePartitions)
//                {
//                    if(isPointInRegion(theRegion, partition->nodes.front().lock()->position))
//                    {
//                        anyInside = true;
//                        break;
//                    }
//                }
//
//                if(!anyInside)
//                {
//                    for(auto partition : outsidePartitions)
//                    {
//                        if(isPointInRegion(theRegion, partition->nodes.front().lock()->position))
//                        {
//                            anyInside = true;
//                            break;
//                        }
//                    }
//                }
//
//                if(!anyInside)
//                    path = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};
//            }
//        }
//        else
//        {
//            bool good = true;
//            shared_ptr<Node> otherNode;
//            if(neighborMap[startNode].size() == 2 && neighborMap[startNode][0] == endNode
//                    && neighborMap[startNode][1] == endNode &&
//                    neighborMap[endNode].size() == 3 && ((neighborMap[endNode][0] == startNode)
//                    + (neighborMap[endNode][1] == startNode) + (neighborMap[endNode][2] == startNode)) == 2)
//            {
//                const vector<shared_ptr<Node>> & nodeList = neighborMap[endNode];
//                auto findNode = startNode;
//                otherNode = *find_if(nodeList.begin(), nodeList.end(), [findNode](shared_ptr<Node> node){return node != findNode;});
//            }
//            else if(neighborMap[endNode].size() == 2 && neighborMap[endNode][0] == startNode
//                    && neighborMap[endNode][1] == startNode &&
//                    neighborMap[startNode].size() == 3 && ((neighborMap[startNode][0] == endNode)
//                    + (neighborMap[startNode][1] == endNode) + (neighborMap[startNode][2] == endNode)) == 2)
//            {
//                const vector<shared_ptr<Node>> & nodeList = neighborMap[startNode];
//                auto findNode = endNode;
//                otherNode = *find_if(nodeList.begin(), nodeList.end(), [findNode](shared_ptr<Node> node){return node != findNode;});
//            }
//            else
//                good = false;
//            if(good)
//            {
//                good = !isPointInRegion(containingRegion, otherNode->position);
//            }
//            if(good)
//            {
//                bool anyInside = false;
//                shared_ptr<Region> theRegion = pointToRegion(gs, 0.5f * (startNode->position + endNode->position));
//
//                if(theRegion == containingRegion && !doesCubicSplineListIntersect(gs, vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)}))
//                {
//                    for(auto partition : insidePartitions)
//                    {
//                        if(isPointInRegion(theRegion, partition->nodes.front().lock()->position))
//                        {
//                            anyInside = true;
//                            break;
//                        }
//                    }
//
//                    if(!anyInside)
//                    {
//                        for(auto partition : outsidePartitions)
//                        {
//                            if(isPointInRegion(theRegion, partition->nodes.front().lock()->position))
//                            {
//                                anyInside = true;
//                                break;
//                            }
//                        }
//                    }
//
//                    if(!anyInside)
//                        path = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};
//                }
//            }
//        }
//    }
    if(startNode != endNode && path.empty() && doesPathWork(vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)}))
    {
        path = vector<CubicSpline> {CubicSpline(startNode->position, endNode->position)};
    }
    if(startNode != endNode && path.empty())
    {
        VectorF delta = endNode->position - startNode->position;
        VectorF sideways = abs(delta) * 1.2f * normalize(cross(delta, VectorF(0, 0, 1)));
        for(size_t i = 0; i < 10; i++)
        {
            float v = (float)rand() / RAND_MAX;
            v = v * 2 - 1;
            vector<CubicSpline> testPath = splinesFromLines(vector<VectorF>{startNode->position, 0.5f * (startNode->position + endNode->position) + sideways * v, endNode->position});
            if(doesPathWork(testPath))
            {
                path = testPath;
                break;
            }
        }
    }
    if(startNode != endNode && path.empty())
    {
        for(size_t i = 0; i < 100; i++)
        {
            VectorF p1 = VectorF((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0) * 3 - VectorF(1.5f);
            VectorF p2 = VectorF((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0) * 3 - VectorF(1.5f);
            vector<CubicSpline> testPath = splinesFromLines(vector<VectorF>{startNode->position, p1, p2, endNode->position});
            if(doesPathWork(testPath))
            {
                path = testPath;
                break;
            }
        }
    }

    if(path.size() == 0)
    {
        throw InvalidMoveException();
    }
    else
    {
        *this = GameStateMove(gs, startNode, endNode, path);
    }
}

Edge::Edge(const vector<CubicSpline> &cubicSplines, shared_ptr<Node> start, shared_ptr<Node> end)
    : cubicSplines(cubicSplines)
{
    this->start = start;
    this->end = end;
    //    assert(cubicSplines[0].p0 == start->position);
    //    assert(cubicSplines.back().p1 == end->position);
}

GameStateMove getAIMove(GameState gs)
{
    vector<GameStateMove> moves = getValidMoves(gs);

    if(moves.size() == 0)
    {
        throw InvalidMoveException();
    }

    for(GameStateMove &m : moves)
    {
#if 0
        try
        {
            GameState gs = (GameState)m;

            if(validMoveCount(gs) == 0) // winning move
            {
                return m;
            }
        }
        catch(InvalidMoveException & e)
        {
        }
#endif
    }

    return moves[rand() % moves.size()];
}


void GameStateMove::dump() const
{
    cout << "Start : " << startNode->position << "\nEnd : " << endNode->position << "\n";
}
