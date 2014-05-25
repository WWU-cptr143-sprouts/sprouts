#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <cassert>
#include <utility>
#include <thread>
#include <chrono>
#include <list>

using namespace std;

struct graph_build_error : public runtime_error
{
    graph_build_error(const string &msg)
        : runtime_error(msg)
    {
    }
};

/**
    class for a directed graph
*/
template <typename NT, typename ET = NT>
class graph
{
private:
    struct Node;
    struct Edge
    {
        ET data;
        bool flag = false;
        Node *src;
        Node *dest;
        Edge(const ET &data)  // copy into data
            : data(data)
        {
        }
        Edge(ET  &&data) // move into data
            : data(data)
        {
        }
    };

    struct Node
    {
        int index;
        NT data;
        bool flag = false;
        vector<Edge *> adjacencyList;
        Node(int index, const NT &data)  // copy into data
            : index(index), data(data)
        {
        }
        Node(int index, NT  &&data) // move into data
            : index(index), data(data)
        {
        }
    };
    vector<Node *> nodes;
    vector<Edge *> edges;
public:
    graph() // default constructor : constructs empty graph
    {
    }
    ~graph() // destructor : destroys graph
    {
        for(Node *node : nodes)
        {
            delete node;
        }

        for(Edge *edge : edges)
        {
            delete edge;
        }
    }
    graph(const graph &rt)  // copy constructor
    {
        map<const Node *, Node *> nodeMap;

        for(const Node *node : rt.nodes)
        {
            Node *newNode = new Node(nodes.size(), node->data);
            nodes.push_back(newNode);
            nodeMap[node] = newNode;
        }

        for(const Node *node : rt.nodes)
        {
            Node *newNode = nodeMap[node];

            for(const Edge *edge : node->adjacencyList)
            {
                Edge *newEdge = new Edge(edge->data);
                newEdge->src = node;
                newEdge->dest = nodeMap[edge->dest];
                newNode->adjacencyList.push_back(newEdge);
                edges.push_back(newEdge);
            }
        }
    }
    graph(graph  &&rt) // move constructor
    {
        swap(nodes, rt.nodes);
        swap(edges, rt.edges);
    }
    const graph &operator =(graph && rt)  // move assignment
    {
        swap(nodes, rt.nodes);
        swap(edges, rt.edges);
        return *this;
    }
    const graph &operator =(const graph &rt)   // copy assignment
    {
        if(&rt == this)
        {
            return *this;
        }

        for(Node *node : nodes)
        {
            delete node;
        }

        nodes.clear();

        for(Edge *edge : edges)
        {
            delete edge;
        }

        edges.clear();
        map<const Node *, Node *> nodeMap;

        for(const Node *node : rt.nodes)
        {
            Node *newNode = new Node(nodes.size(), node->data);
            nodes.push_back(newNode);
            nodeMap[node] = newNode;
        }

        for(const Node *node : rt.nodes)
        {
            Node *newNode = nodeMap[node];

            for(const Edge *edge : node->adjacencyList)
            {
                Edge *newEdge = new Edge(edge->data);
                newEdge->src = node;
                newEdge->dest = nodeMap[edge->dest];
                newNode->adjacencyList.push_back(newEdge);
                edges.push_back(newEdge);
            }
        }

        return *this;
    }
    void clear()
    {
        for(Node *node : nodes)
        {
            delete node;
        }

        nodes.clear();

        for(Edge *edge : edges)
        {
            delete edge;
        }

        edges.clear();
    }
    void build() // builds graph interactively
    {
        clear();
        cout << "Enter the number of nodes:";
        int nodeCount;

        if(!(cin >> nodeCount) || nodeCount < 0)
        {
            throw graph_build_error("invalid node count");
        }

        vector<vector<pair<Edge *, int> > > adjacencyLists;
        adjacencyLists.resize(nodeCount);

        try
        {
            for(int i = 0; i < nodeCount; i++)
            {
                cout << "Node #" << (i + 1) << ":\n";
                cout << "Enter Node Data:";
                NT data;

                if(!(cin >> data))
                {
                    throw graph_build_error("invalid node data");
                }

                Node *newNode = new Node(nodes.size(), move(data));  // move if we can because it's more efficient
                nodes.push_back(newNode);

                for(;;)
                {
                    cout << "Enter adjacent node (or 0 to end):";
                    int n;

                    if(!(cin >> n) || n < 0 || n > nodeCount)
                    {
                        throw graph_build_error("invalid node adjacency");
                    }

                    if(n == 0)
                    {
                        break;
                    }

                    for(auto edge : adjacencyLists[i])
                    {
                        if(get<1>(edge) == n - 1)
                        {
                            throw graph_build_error("duplicate node adjacency");
                        }
                    }

                    ET data;
                    cout << "Enter edge data:";

                    if(!(cin >> data))
                    {
                        throw graph_build_error("invalid edge data");
                    }

                    Edge *newEdge;
                    newEdge = new Edge(move(data)); // move if we can because it's more efficient
                    adjacencyLists[i].push_back(make_pair(newEdge, n - 1));
                }
            }
        }
        catch(...)
        {
            for(auto adjacencyList : adjacencyLists)
            {
                for(auto edgePair : adjacencyList)
                {
                    delete get<0>(edgePair);
                }
            }

            throw;
        }

        for(int i = 0; i < nodeCount; i++)
        {
            Node *node = nodes[i];

            for(auto edgePair : adjacencyLists[i])
            {
                int destIndex = get<1>(edgePair);
                Edge *edge = get<0>(edgePair);
                edge->src = node;
                edge->dest = nodes[destIndex];
                node->adjacencyList.push_back(edge);
            }
        }
    }
    friend class node_iterator;
    class node_iterator : public iterator<random_access_iterator_tag, NT> // node iterator
    {
        friend class graph;
    private:
        int index;
        const graph *g;
        Node *get() const
        {
            return g->nodes[index];
        }
    public:
        node_iterator()
        {
        }
        node_iterator(int index, const graph *g)
            : index(index), g(g)
        {
        }
        friend bool operator ==(const node_iterator &l, const node_iterator &r)
        {
            return l.index == r.index;
        }
        friend bool operator !=(const node_iterator &l, const node_iterator &r)
        {
            return l.index != r.index;
        }
        const NT *operator ->() const
        {
            return &g->nodes[index]->data;
        }
        const NT &operator *() const
        {
            return g->nodes[index]->data;
        }
        const node_iterator &operator ++()
        {
            ++index;
            return *this;
        }
        node_iterator operator ++(int)
        {
            return node_iterator(index++, g);
        }
        const node_iterator &operator --()
        {
            --index;
            return *this;
        }
        node_iterator operator --(int)
        {
            return node_iterator(index--, g);
        }
        friend node_iterator operator +(int v, const node_iterator &iter)
        {
            return node_iterator(v + iter.index, iter.g);
        }
        friend node_iterator operator +(const node_iterator &iter, int v)
        {
            return node_iterator(iter.index + v, iter.g);
        }
        friend node_iterator operator -(const node_iterator &iter, int v)
        {
            return node_iterator(iter.index - v, iter.g);
        }
        friend int operator -(const node_iterator &l, const node_iterator &r)
        {
            return l.index - r.index;
        }
        friend bool operator <(const node_iterator &l, const node_iterator &r)
        {
            return l.index < r.index;
        }
        friend bool operator <=(const node_iterator &l, const node_iterator &r)
        {
            return l.index <= r.index;
        }
        friend bool operator >=(const node_iterator &l, const node_iterator &r)
        {
            return l.index >= r.index;
        }
        friend bool operator >(const node_iterator &l, const node_iterator &r)
        {
            return l.index > r.index;
        }
        const node_iterator &operator +=(int v)
        {
            index += v;
            return *this;
        }
        const node_iterator &operator -=(int v)
        {
            index -= v;
            return *this;
        }
        const NT &operator [](int index2) const
        {
            return g->nodes[index + index2]->data;
        }
        const int position() const
        {
            return index;
        }
    };
    class edge_iterator : public
        iterator<random_access_iterator_tag, pair<const ET &, node_iterator> > // edge iterator
    {
    private:
        typename vector<Edge *>::const_iterator iter;
        const graph *g;
    public:
        edge_iterator()
        {
        }
        edge_iterator(typename vector<Edge *>::const_iterator iter, const graph *g)
            : iter(iter), g(g)
        {
        }
        friend bool operator ==(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter == r.iter;
        }
        friend bool operator !=(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter != r.iter;
        }
        pair<const ET &, node_iterator> operator *() const
        {
            return pair<const ET &, node_iterator>((*iter)->data, node_iterator((*iter)->dest->index, g));
        }
        const edge_iterator &operator ++()
        {
            ++iter;
            return *this;
        }
        edge_iterator operator ++(int)
        {
            return edge_iterator(iter++, g);
        }
        const edge_iterator &operator --()
        {
            --iter;
            return *this;
        }
        edge_iterator operator --(int)
        {
            return edge_iterator(iter--, g);
        }
        friend edge_iterator operator +(int v, const edge_iterator &iter)
        {
            return edge_iterator(v + iter.iter, iter.g);
        }
        friend edge_iterator operator +(const edge_iterator &iter, int v)
        {
            return edge_iterator(iter.iter + v, iter.g);
        }
        friend edge_iterator operator -(const edge_iterator &iter, int v)
        {
            return edge_iterator(iter.iter - v, iter.g);
        }
        friend int operator -(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter - r.iter;
        }
        friend bool operator <(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter < r.iter;
        }
        friend bool operator <=(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter <= r.iter;
        }
        friend bool operator >=(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter >= r.iter;
        }
        friend bool operator >(const edge_iterator &l, const edge_iterator &r)
        {
            return l.iter > r.iter;
        }
        const edge_iterator &operator +=(int v)
        {
            iter += v;
            return *this;
        }
        const edge_iterator &operator -=(int v)
        {
            iter -= v;
            return *this;
        }
        pair<const ET &, node_iterator> operator [](int index) const
        {
            return *(*this + index);
        }
        node_iterator src() const
        {
            return node_iterator((*iter)->src->index, g);
        }
        node_iterator dest() const
        {
            return node_iterator((*iter)->dest->index, g);
        }
    };
    node_iterator begin() const // return iterator to first node
    {
        return node_iterator(0, this);
    }
    node_iterator cbegin() const // return iterator to first node
    {
        return node_iterator(0, this);
    }
    node_iterator end() const // return iterator to past-the-end node
    {
        return node_iterator(nodes.size(), this);
    }
    node_iterator cend() const // return iterator to past-the-end node
    {
        return node_iterator(nodes.size(), this);
    }
    edge_iterator begin(node_iterator ni) const // return iterator to first edge
    {
        return edge_iterator(nodes[ni.index]->adjacencyList.begin(), this);
    }
    edge_iterator cbegin(node_iterator ni) const // return iterator to first edge
    {
        return edge_iterator(nodes[ni.index]->adjacencyList.begin(), this);
    }
    edge_iterator end(node_iterator ni) const // return iterator to past-the-end edge
    {
        return edge_iterator(nodes[ni.index]->adjacencyList.end(), this);
    }
    edge_iterator cend(node_iterator ni) const // return iterator to past-the-end edge
    {
        return edge_iterator(nodes[ni.index]->adjacencyList.end(), this);
    }
private:
    void isGraphConnectedDepthFirstHelper(Node *node) const  // depth first search helper
    {
        node->flag = true;

        for(Edge *edge : node->adjacencyList)
        {
            if(!edge->dest->flag)
            {
                isGraphConnectedDepthFirstHelper(edge->dest);
            }
        }
    }
    void unmarkAll() const // clear all flags
    {
        for(Node *node : nodes)
        {
            node->flag = false;
        }
    }
    bool anyFlagMatches(bool state) const // return if any node's flag matches state
    {
        for(Node *node : nodes)
        {
            if(state == node->flag)
            {
                return true;
            }
        }

        return false;
    }
    bool anyMarked() const // return if any nodes are marked
    {
        return anyFlagMatches(true);
    }
    bool allMarked() const // return if all nodes are marked
    {
        return !anyFlagMatches(false);
    }
public:
    bool isGraphConnectedDepthFirst()
    const // return if the graph is connected starting from the first node
    {
        if(nodes.empty())
        {
            return true;
        }

        unmarkAll();
        isGraphConnectedDepthFirstHelper(nodes.front());
        return allMarked();
    }
    bool isGraphConnectedBreadthFirst()
    const // return if the graph is connected starting from the first node
    {
        if(nodes.empty())
        {
            return true;
        }

        unmarkAll();
        deque<Node *> nodeQueue;
        nodeQueue.push_back(nodes.front());

        while(!nodeQueue.empty())
        {
            Node *node = nodeQueue.front();
            nodeQueue.pop_front();
            node->flag = true;

            for(Edge *edge : node->adjacencyList)
            {
                if(!edge->dest->flag)
                {
                    nodeQueue.push_back(edge->dest);
                }
            }
        }

        return allMarked();
    }
    node_iterator addNode(const NT &data)
    {
        node_iterator retval(nodes.size(), this);
        nodes.push_back(new Node(nodes.size(), data));
        return retval;
    }
    node_iterator addNode(NT  &&data)
    {
        node_iterator retval(nodes.size(), this);
        nodes.push_back(new Node(nodes.size(), move(data)));
        return retval;
    }
    bool hasEdge(node_iterator src, node_iterator dest) const
    {
        if(src == end())
        {
            return false;
        }

        if(dest == end())
        {
            return false;
        }

        for(edge_iterator i = begin(src); i != end(src); i++)
        {
            if(get<1>(*i) == dest)
            {
                return true;
            }
        }

        return false;
    }
    edge_iterator addEdge(const ET &data, node_iterator src, node_iterator dest)
    {
        assert(src != end() && dest != end());

        for(edge_iterator i = begin(src); i != end(src); i++)
        {
            if(get<1>(*i) == dest)
            {
                assert(false);
            }
        }

        Edge *newEdge = new Edge(data);
        newEdge->src = src.get();
        newEdge->dest = dest.get();
        edges.push_back(newEdge);
        newEdge->src->adjacencyList.push_back(newEdge);
        return edge_iterator(begin(src) + (newEdge->src->adjacencyList.size() - 1));
    }
    edge_iterator addEdge(ET  &&data, node_iterator src, node_iterator dest)
    {
        assert(src != end() && dest != end());

        for(edge_iterator i = begin(src); i != end(src); i++)
        {
            if(get<1>(*i) == dest)
            {
                assert(false);
            }
        }

        Edge *newEdge = new Edge(move(data));
        newEdge->src = src.get();
        newEdge->dest = dest.get();
        edges.push_back(newEdge);
        newEdge->src->adjacencyList.push_back(newEdge);
        return edge_iterator(begin(src) + (newEdge->src->adjacencyList.size() - 1));
    }
    template <typename NT2, typename ET2>
    friend ostream &operator <<(ostream &os, const graph<NT2, ET2> &g);
    size_t nodeCount() const
    {
        return nodes.size();
    }
    size_t edgeCount() const
    {
        return edges.size();
    }
};

template <typename NT, typename ET>
inline vector<vector<typename graph<NT, ET>::node_iterator> > findShortestPath(const graph<NT, ET> & g, typename graph<NT, ET>::node_iterator sourceNode) // finds shortest path from sourceNode to all other nodes using dijkstra's algorithm
{
    if(sourceNode == g.end())
        return vector<vector<typename graph<NT, ET>::node_iterator> >();
    vector<ET> distance;
    const ET infinity = (ET)-1; // special value for infinity
    distance.resize(g.nodeCount(), infinity);
    distance[sourceNode.position()] = 0;
    vector<typename graph<NT, ET>::node_iterator> previous;
    previous.resize(g.nodeCount(), g.end());
    previous[sourceNode.position()] = sourceNode;

    list<typename graph<NT, ET>::node_iterator> nodesLeft;
    for(size_t i = 0; i < g.nodeCount(); i++)
    {
        nodesLeft.push_back(g.begin() + i);
    }

    while(!nodesLeft.empty())
    {
        ET minDistance = infinity;
        auto minIterator = nodesLeft.end();
        for(auto i = nodesLeft.begin(); i != nodesLeft.end(); i++)
        {
            ET curDistance = distance[i->position()];
            if(curDistance == minDistance || minDistance == infinity || (curDistance != infinity && curDistance <= minDistance))
            {
                minDistance = curDistance;
                minIterator = i;
            }
        }
        if(minDistance == infinity)
            break;
        auto node = *minIterator;
        nodesLeft.erase(minIterator);
        for(auto edge = g.begin(node); edge != g.end(node); edge++)
        {
            ET alternateDistance = distance[node.position()];
            if(alternateDistance != infinity)
                alternateDistance += get<0>(*edge);
            auto destNode = get<1>(*edge);
            if(distance[destNode.position()] == alternateDistance || distance[destNode.position()] == infinity || (alternateDistance != infinity && alternateDistance <= distance[destNode.position()]))
            {
                distance[destNode.position()] = alternateDistance;
                previous[destNode.position()] = node;
            }
        }
    }

    vector<vector<typename graph<NT, ET>::node_iterator> > retval;
    retval.resize(g.nodeCount());
    for(size_t i = 0; i < g.nodeCount(); i++)
    {
        if(previous[i] == g.end())
            continue;
        auto node = g.begin() + i;
        retval[i].push_back(node);
        while(node != sourceNode)
        {
            node = previous[node.position()];
            retval[i].push_back(node);
        }
        reverse(retval[i].begin(), retval[i].end());
    }
    return retval;
}

inline graph<int> generateRandomGraph(int numberOfNodes, int numberOfEdges, int minimumEdgeValue,
                               int maximumEdgeValue)
{
    assert(numberOfEdges <= numberOfNodes * numberOfNodes);
    graph<int> retval;

    for(int i = 0; i < numberOfNodes; i++)
    {
        retval.addNode(i + 1);
    }

    for(int i = 0; i < numberOfEdges; i++)
    {
        int srcNode = rand() % numberOfNodes;
        int destNode = rand() % numberOfNodes;
        int testCount = 0;

        while(retval.hasEdge(retval.begin() + srcNode, retval.begin() + destNode))
        {
            if(++testCount > numberOfNodes * numberOfNodes)
            {
                assert(false);
            }

            srcNode++;
            destNode += srcNode / numberOfNodes;
            destNode %= numberOfNodes;
            srcNode %= numberOfNodes;
        }

        retval.addEdge(rand() % (maximumEdgeValue - minimumEdgeValue + 1) + minimumEdgeValue,
                       retval.begin() + srcNode, retval.begin() + destNode);
    }

    return move(retval);
}

namespace GraphingInternals
{
template <typename NT, typename ET>
struct DrawGraphEdge
{
    typename graph<NT, ET>::edge_iterator edge;
    string text;
    int x = 0;
    DrawGraphEdge(typename graph<NT, ET>::edge_iterator edge)
        : edge(edge)
    {
        ostringstream os;
        os << get<0>(*edge);
        text = os.str();

        if(text != "")
        {
            text += " ";
        }
    }
    DrawGraphEdge()
    {
    }
    bool intersectsY(const DrawGraphEdge &other) const
    {
        int y1s = edge.src().position();
        int y1e = edge.dest().position();

        if(y1s > y1e)
        {
            swap(y1s, y1e);
        }

        int y2s = other.edge.src().position();
        int y2e = other.edge.dest().position();

        if(y2s > y2e)
        {
            swap(y2s, y2e);
        }

        if(y1s > y2s)
        {
            swap(y1s, y2s);
            swap(y1e, y2e);
        }

        if(y2s == y2e && y1s < y2s && y1e >= y2s)
        {
            return true;
        }

        if(y2s >= y1e)
        {
            return false;
        }

        return true;
    }
    bool operator ==(const DrawGraphEdge &rt) const
    {
        return edge == rt.edge;
    }
    bool operator !=(const DrawGraphEdge &rt) const
    {
        return edge != rt.edge;
    }
    bool operator <(const DrawGraphEdge &rt) const
    {
        if(*this == rt)
        {
            return false;
        }

        return abs(edge.src().position() - edge.dest().position()) < abs(rt.edge.src().position() -
                rt.edge.dest().position());
    }
    bool operator >(const DrawGraphEdge &rt) const
    {
        if(*this == rt)
        {
            return false;
        }

        return abs(edge.src().position() - edge.dest().position()) > abs(rt.edge.src().position() -
                rt.edge.dest().position());
    }
    bool operator <=(const DrawGraphEdge &rt) const
    {
        if(*this == rt)
        {
            return true;
        }

        return abs(edge.src().position() - edge.dest().position()) < abs(rt.edge.src().position() -
                rt.edge.dest().position());
    }
    bool operator >=(const DrawGraphEdge &rt) const
    {
        if(*this == rt)
        {
            return true;
        }

        return abs(edge.src().position() - edge.dest().position()) > abs(rt.edge.src().position() -
                rt.edge.dest().position());
    }
};
inline void drawTextBlock(ostream &os, const vector<vector<string> > &textBlock, int lineLength)
{
    os << "\n";

    for(int y = 0; y < (int)textBlock.size(); y++)
    {
        string line = "";

        for(int x = 0; x < lineLength; x++)
        {
            line += textBlock[y][x];
        }

        os << line << "\n";
    }
}
}

template <typename NT, typename ET>
inline ostream &operator <<(ostream &os, const graph<NT, ET> &g)
{
    using namespace GraphingInternals;
    vector<string> nodeText;

    for(const NT &nv : g)
    {
        ostringstream oss;
        oss << nv;
        nodeText.push_back(oss.str());
    }

    vector<DrawGraphEdge<NT, ET> > edges;
    int totalSize = 0;

    for(auto ni = g.begin(); ni != g.end(); ni++)
    {
        for(auto ei = g.begin(ni); ei != g.end(ni); ei++)
        {
            DrawGraphEdge<NT, ET> e(ei);
            totalSize += 5 + e.text.size();
            edges.push_back(e);
        }
    }

    sort(edges.begin(), edges.end());
    vector<bool> possibleXValues;
    vector<int> maxX;
    possibleXValues.resize(totalSize);
    maxX.resize(nodeText.size(), -1);

    for(typename vector<DrawGraphEdge<NT, ET> >::iterator i = edges.begin(); i != edges.end(); i++)
    {
        for(auto i = possibleXValues.begin(); i != possibleXValues.end(); i++)
        {
            *i = true;
        }

        for(typename vector<DrawGraphEdge<NT, ET> >::const_iterator j = edges.begin(); j != i; j++)
        {
            if(i->intersectsY(*j))
            {
                for(int k = 2 + j->text.size(); k >= 0; k--)
                {
                    possibleXValues[j->x + k] = false;
                }
            }
        }

        int count = 0;

        for(i->x = 0; i->x < possibleXValues.size(); i->x++)
        {
            if(possibleXValues[i->x])
            {
                if(++count >= i->text.size() + 2)
                {
                    break;
                }
            }
            else
            {
                count = 0;
            }
        }

        i->x -= count - 1;
        int y1s = i->edge.src().position();
        int y1e = i->edge.dest().position();

        if(y1s != y1e)
        {
            maxX[y1s] = max(maxX[y1s], i->x);
            maxX[y1e] = max(maxX[y1e], i->x);
        }
    }

    int xOffset = 0;

    for(string s : nodeText)
    {
        xOffset = max<int>(xOffset, s.size());
    }

    xOffset += 3;
    vector<vector<string> > textBlock;
    textBlock.resize(3 * nodeText.size());

    for(vector<string> &v : textBlock)
    {
        v.resize(xOffset + totalSize, " ");
    }

    for(int y = 0; y < nodeText.size(); y++)
    {
        int x;

        for(x = 0; x < nodeText[y].size(); x++)
        {
            textBlock[y * 3 + 1][x + 2] = string(1, nodeText[y][x]);
        }

        if(maxX[y] >= 0)
        {
            for(x += 2; x < xOffset + maxX[y]; x++)
            {
                textBlock[y * 3 + 1][x] = "─";
            }
        }
    }

    for(DrawGraphEdge<NT, ET> edge : edges)
    {
        int y1s = edge.edge.src().position();
        int y1e = edge.edge.dest().position();

        if(y1s == y1e)
        {
            textBlock[y1s * 3 + 0][0] = "┌";
            textBlock[y1s * 3 + 0][1] = "─";
            textBlock[y1s * 3 + 0][2] = "┐";
            textBlock[y1s * 3 + 1][0] = "│";
            textBlock[y1s * 3 + 2][0] = "└";
            textBlock[y1s * 3 + 2][1] = "─";
            textBlock[y1s * 3 + 2][2] = "^";

            for(int x = 0; x < edge.text.size(); x++)
            {
                textBlock[y1s * 3][x + 3] = string(1, edge.text[x]);
            }
        }
        else if(y1s < y1e)
        {
            for(int y = y1s * 3 + 2; y < y1e * 3; y++)
            {
                textBlock[y][edge.x + xOffset] = "│";
            }

            if(textBlock[y1s * 3 + 1][edge.x + xOffset] == "─")
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┬";
            }
            else if(textBlock[y1s * 3 + 1][edge.x + xOffset] == "┴")
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┼";
            }
            else if(textBlock[y1s * 3 + 1][edge.x + xOffset] == "┘")
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┤";
            }
            else
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┐";
            }

            textBlock[y1e * 3][edge.x + xOffset] = "v";

            for(int x = 0; x < edge.text.size(); x++)
            {
                textBlock[y1s * 3 + 2][edge.x + xOffset + x + 1] = string(1, edge.text[x]);
            }
        }
        else
        {
            for(int y = y1e * 3 + 2; y < y1s * 3 + 1; y++)
            {
                textBlock[y][edge.x + xOffset] = "│";
            }

            if(textBlock[y1s * 3 + 1][edge.x + xOffset] == "─")
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┴";
            }
            else if(textBlock[y1s * 3 + 1][edge.x + xOffset] == "┬")
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┼";
            }
            else if(textBlock[y1s * 3 + 1][edge.x + xOffset] == "┐")
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┤";
            }
            else
            {
                textBlock[y1s * 3 + 1][edge.x + xOffset] = "┘";
            }

            textBlock[y1e * 3 + 2][edge.x + xOffset] = "^";

            for(int x = 0; x < edge.text.size(); x++)
            {
                textBlock[y1s * 3][edge.x + xOffset + x + 1] = string(1, edge.text[x]);
            }
        }
    }

    for(int y = 0; y < textBlock.size(); y++)
    {
        for(int x = xOffset + totalSize - 1; x > 0; x--)
        {
            if(textBlock[y][x] == " " && textBlock[y][x - 1] == "─")
            {
                textBlock[y][x] = "╴";
            }
        }

        for(int x = xOffset + totalSize - 1; x >= 0; x--)
        {
            string &s = textBlock[y][x];

            if(s == " ")
            {
                s = "";
            }
            else
            {
                break;
            }
        }
    }

    drawTextBlock(os, textBlock, xOffset + totalSize);
    return os;
}

template <typename NT, typename ET>
inline graph<NT, ET> findAllShortestPaths(const graph<NT, ET> & g, const ET infinity = (ET)-1)
{
    vector<vector<ET> > dist;
    dist.resize(g.nodeCount());
    for(vector<ET> & v : dist)
    {
        v.resize(g.nodeCount(), infinity);
    }
    for(size_t i = 0; i < g.nodeCount(); i++)
    {
        auto node = g.begin() + i;
        for(auto edge = g.begin(node); edge != g.end(node); edge++)
        {
            dist[i][edge.dest().position()] = get<0>(*edge);
        }
        dist[i][i] = 0;
    }
    for(size_t k = 0; k < g.nodeCount(); k++)
    {
        for(size_t i = 0; i < g.nodeCount(); i++)
        {
            for(size_t j = 0; j < g.nodeCount(); j++)
            {
                ET sumDist = dist[i][k];
                if(sumDist != infinity && dist[k][j] != infinity)
                    sumDist = dist[i][k] + dist[k][j];
                else
                    sumDist = infinity;
                if(sumDist != infinity && (dist[i][j] == infinity || dist[i][j] > sumDist))
                {
                    dist[i][j] = sumDist;
                }
            }
        }
    }
    graph<NT, ET> retval;
    for(size_t i = 0; i < g.nodeCount(); i++)
    {
        retval.addNode(g.begin()[i]);
    }
    for(size_t i = 0; i < g.nodeCount(); i++)
    {
        for(size_t j = 0; j < g.nodeCount(); j++)
        {
            if(dist[i][j] != infinity)
            {
                retval.addEdge(move(dist[i][j]), retval.begin() + i, retval.begin() + j);
            }
        }
    }
    return move(retval);
}
#endif // GRAPH_H_INCLUDED
