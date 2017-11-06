//Copyright (c) 2017 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#ifndef MINIMUMSPANNINGTREE_H
#define MINIMUMSPANNINGTREE_H

#include <vector>
#include <unordered_map>

#include "intpoint.h"

namespace cura
{

/*!
 * \brief Implements Prim's algorithm to compute Minimum Spanning Trees (MST).
 *
 * The minimum spanning tree is always computed from a clique of vertices.
 */
class MinimumSpanningTree
{
    /*!
     * \brief Represents an edge of the tree.
     *
     * While edges are meant to be undirected, these do have a start and end
     * point.
     */
    struct Edge {
        /**
         * Constructs a new edge.
         * @param start One endpoint of the edge.
         * @param end The other endpoint of the edge.
         */
        Edge(const Point start, const Point end);

        /**
         * The point at which this edge starts.
         */
        const Point start;

        /**
         * The point at which this edge ends.
         */
        const Point end;

        /**
         * Computes the length of this edge.
         * @return The length of this edge.
         */
        int length() const;
    };
public:
    /*!
     * \brief Constructs a minimum spanning tree that spans all given vertices.
     */
    MinimumSpanningTree(const std::vector<Point> vertices);

private:
    const std::unordered_map<Point, std::vector<Edge>> adjacency_graph;

    /*!
     * \brief Computes the edges of a minimum spanning tree using Prim's
     * algorithm.
     *
     * \param vertices The vertices to span.
     * \return An adjacency graph with for each point one or more edges.
     */
    const std::unordered_map<Point, std::vector<Edge>> prim(const std::vector<Point>& vertices) const;
};

}

#endif /* MINIMUMSPANNINGTREE_H */
