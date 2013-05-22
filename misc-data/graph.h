/*
 * An undirected graph using adjacency lists. This uses the item itself
 * as the index, so it must be unique. If not, use a vector to store the
 * values and insert the index of the item in the vector into the graph.
 */

#ifndef H_GRAPH
#define H_GRAPH

#include <map>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>

template<class T> class Graph
{
    std::map<T, std::list<T> > list;
    
    // Decreases typing
    typedef typename std::list<T>::iterator list_iter;
    typedef typename std::map<T, std::list<T> >::iterator map_iter;
    typedef typename std::list<T>::const_iterator list_citer;
    typedef typename std::map<T, std::list<T> >::const_iterator map_citer;

public:
    Graph() { }

    // Insert a vertex. If we allowed inserted edges here then we'd
    // have to verify they all exist. Might as well just use insertEdge()
    void insert(const T&);

    // Insert an edge from two vertices. Returns true if
    // successfully inserted (i.e., the vertices exist).
    bool insert(const T&, const T&);

    // Delete the vertex and any edges to it. Return true
    // if deleted.
    bool erase(const T&);

    // Delete an edge. Returns if deleted successfully.
    bool erase(const T&, const T&);

    // Determine if the graph is connected. Specify item to start at.
    bool connectedDFS(const T&) const;
    bool connectedBFS(const T&) const;

    // Output this in adjacency list form for debugging
    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const Graph<U>& graph);
};

// Just add this to the list
template<class T> void Graph<T>::insert(const T& item)
{
    list[item] = std::list<T>();
}

// If the vertices exist, insert an edge between them.
template<class T> bool Graph<T>::insert(const T& v1, const T& v2)
{
    map_iter pos1 = list.find(v1);
    map_iter pos2 = list.find(v2);

    // Vertices don't exist
    if (pos1 == list.end() || pos2 == list.end())
        return false;

    // Only insert edge vertex if not already in the list
    list_citer edge_pos1 = std::find(pos1->second.begin(), pos1->second.end(), v2);
    list_citer edge_pos2 = std::find(pos2->second.begin(), pos2->second.end(), v1);

    if (edge_pos1 != pos1->second.end() || edge_pos2 != pos2->second.end())
        return false;

    pos1->second.push_back(v2);
    pos2->second.push_back(v1);

    return true;
}

// Since this is an undirected graph we can look at the edges in list[v],
// delete these, and also delete all the references in the edges listed
// in list[v].
template<class T> bool Graph<T>::erase(const T& v)
{
    map_iter pos = list.find(v);

    if (pos == list.end())
        return false;

    // Delete edges to this vertex
    for (list_iter i = pos->second.begin(); i != pos->second.end(); ++i)
    {
        map_iter edges = list.find(*i);
        list_iter item = std::find(edges->second.begin(), edges->second.end(), v);

        if (item != edges->second.end())
            edges->second.erase(item);
    }

    // Delete this vertex (and in the process edges from this vertex)
    list.erase(pos);

    return true;
}

// Just delete the two entries if they exist
template<class T> bool Graph<T>::erase(const T& v1, const T& v2)
{
    map_iter pos1 = list.find(v1);
    map_iter pos2 = list.find(v2);

    if (pos1 == list.end() || pos2 == list.end())
        return false;

    list_iter item1 = std::find(pos1->second.begin(), pos1->second.end(), v2);
    list_iter item2 = std::find(pos2->second.begin(), pos2->second.end(), v1);

    // As long as there aren't any bugs in this class, we don't need to check
    // if the item was found since it will have been.
    pos1->second.erase(item1);
    pos2->second.erase(item2);

    return true;
}

// Compare number of vertices we traversed in a depth-first search to the total
// number of vertices.
template<class T> bool Graph<T>::connectedDFS(const T& start) const
{
    std::stack<T> stack;
    std::vector<T> visited;

    map_citer first = list.find(start);

    // Don't add this first one if it doesn't exist
    if (first == list.end())
        return false;
    
    stack.push(start);

    do
    {
        T item = stack.top();
        stack.pop();

        // Skip if we've been here already. Do it here so we don't have to check
        // both whether it's in the queue or in the visited array.
        if (std::find(visited.begin(), visited.end(), item) != visited.end())
            continue;
        
        visited.push_back(item);
        map_citer pos = list.find(item);

        // Insert any connected
        if (pos != list.end())
            for (list_citer i = pos->second.begin(); i != pos->second.end(); ++i)
                stack.push(*i);
    } while(!stack.empty());

    // If it's connected, we'll have visited them all
    return list.size()==visited.size();
}

// Same approach as with connectedDFS()
template<class T> bool Graph<T>::connectedBFS(const T& start) const
{
    std::queue<T> queue;
    std::vector<T> visited;

    map_citer first = list.find(start);

    // Don't add this first one if it doesn't exist
    if (first == list.end())
        return false;

    queue.push(start);

    do
    {
        T item = queue.front();
        queue.pop();

        // Skip if we've been here already. Do it here so we don't have to check
        // both whether it's in the queue or in the visited array.
        if (std::find(visited.begin(), visited.end(), item) != visited.end())
            continue;
        
        visited.push_back(item);
        map_citer pos = list.find(item);

        // Insert any connected here
        if (pos != list.end())
            for (list_citer i = pos->second.begin(); i != pos->second.end(); ++i)
                queue.push(*i);
    } while(!queue.empty());

    // If it's connected, we'll have visited them all
    return list.size()==visited.size();
}

// For debugging
template<class T> std::ostream& operator<<(std::ostream& os, const Graph<T>& graph)
{
    for (typename Graph<T>::map_citer i = graph.list.begin(); i != graph.list.end(); ++i)
    {
        os << i->first << ": ";

        for (typename Graph<T>::list_citer j = i->second.begin(); j != i->second.end(); ++j)
        {
            os << *j << " ";
        }

        os << std::endl;
    }

    return os;
}

#endif
