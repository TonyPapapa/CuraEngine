//Copyright (c) 2017 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#include "MinimumSpanningTree.h"

namespace cura
{

MinimumSpanningTree::MinimumSpanningTree(const std::vector<Point> vertices) : adjacency_graph(prim(vertices))
{
    //Just copy over the fields.
}

int MinimumSpanningTree::Edge::length() const
{
    return vSize2(start - end);
}

const std::unordered_map<Point, std::vector<MinimumSpanningTree::Edge>> MinimumSpanningTree::prim(const std::vector<Point>& vertices) const
{
    std::unordered_map<Point, std::vector<Edge>> result;
    if (vertices.empty())
    {
        return result; //No vertices, so we can't create edges either.
    }
    result.reserve(vertices.size());

    Point first_point = vertices[0];
    result[first_point] = std::vector<MinimumSpanningTree::Edge>(); //Start with one vertex in the tree.
    
    std::unordered_map<Point*, coord_t> smallest_distance; //The shortest distance to the current tree.
    smallest_distance.reserve(vertices.size());
    std::unordered_map<Point*, Point*> smallest_distance_to; //Which point the shortest distance goes towards.
    smallest_distance_to.reserve(vertices.size());
    for(Point vertex : vertices)
    {
        if (vertex == first_point)
        {
            continue;
        }
        smallest_distance[&vertex] = vSize2(vertex - first_point);
        smallest_distance_to[&vertex] = &first_point;
    }

    while(result.size() < vertices.size()) //All of the vertices need to be in the tree at the end.
    {
        //Choose the closest vertex to connect to.
        //This search is O(V) right now, which can be made down to O(log(V)). This reduces the overall time complexity from O(V*V) to O(V*log(E)).
        //However that requires an implementation of a heap that supports the decreaseKey operation, which is not in the std library.
        //TODO: Implement this?
        Point* closest_point = nullptr;
        coord_t closest_distance = std::numeric_limits<coord_t>::max();
        for(std::pair<Point*, coord_t> point_and_distance : smallest_distance)
        {
            if (point_and_distance.second < closest_distance) //This one's closer!
            {
                closest_point = point_and_distance.first;
                closest_distance = point_and_distance.second;
            }
        }

        //Add this point to the graph and remove it from the candidates.
        Point closest_point_local = *closest_point;
        if (result.find(closest_point_local) == result.end())
        {
            result[closest_point_local] = std::vector<Edge>();
        }
        result[closest_point_local].emplace_back(closest_point_local, *smallest_distance_to[closest_point]);
        smallest_distance.erase(closest_point); //Remove it so we don't check for these points again.
        smallest_distance_to.erase(closest_point);

        //Update the distances of all points that are not in the graph.
        for (std::pair<Point*, coord_t> point_and_distance : smallest_distance)
        {
            coord_t new_distance = vSize2(*closest_point - *point_and_distance.first);
            if (new_distance < point_and_distance.second) //New point is closer.
            {
                smallest_distance[point_and_distance.first] = new_distance;
                smallest_distance_to[point_and_distance.first] = closest_point;
            }
        }
    }

    return result;
}

}