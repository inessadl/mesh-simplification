//
// Created by inessa on 01/04/16.
//

#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include <meshsimplification.hpp>
#include <iostream>

/* Compares the value of two vertex */
bool MeshSimplification::comp(const std::vector<float>  &a, const std::vector<float>  &b)
{
    return a[0] < b[0];
}

/* Remove a vertex using the Half-Edge Collapse technique, which reconnect all
 * the connections of the choosen vertex to another vertex.
 *
 * The heuristic defines that the vertex to be removed will be the one with
 * more connections from the smallest edge.
 *
 * - calculates the edges
 * - get the vertex of the smallest edge
 * - decide which of the vertex will be removed
 * - uses half-edge collapse for simplification
 * - removes the "line-triangles"
 */
void MeshSimplification::removeVertex(std::vector<unsigned short> &indices, std::vector<glm::vec3> indexed_vertex)
{
    std::vector<std::vector<float> > arestas;
    std::vector<float> x;

    // Calculates the size of all edges
    for (int i = 0; i < indices.size(); i+=3)
    {
        // Calculates and add the edge values and the index for its vertex on the vector/matrix "arestas"
        float edge1 = glm::distance(indexed_vertex[indices[i]], indexed_vertex[indices[i+1]]);
        arestas.push_back(x);
        arestas[i].push_back(edge1);
        arestas[i].push_back((float)i);
        arestas[i].push_back((float)i+1);

        float edge2 = glm::distance(indexed_vertex[indices[i]], indexed_vertex[indices[i+2]]);
        arestas.push_back(x);
        arestas[i+1].push_back(edge2);
        arestas[i+1].push_back((float)i);
        arestas[i+1].push_back((float)i+2);

        float edge3 = glm::distance(indexed_vertex[indices[i+1]], indexed_vertex[indices[i+2]]);
        arestas.push_back(x);
        arestas[i+2].push_back(edge3);
        arestas[i+2].push_back((float)i+1);
        arestas[i+2].push_back((float)i+2);

    }

    // Sort the vector/matrix of edges (arestas) by the smallest edge
    std::sort(arestas.begin(), arestas.end(), MeshSimplification::comp);

    // Go through the "indices" vector to find the value of the two vertex and store on vertex1 and vertex2
    unsigned short vertex1_index = (unsigned short)arestas[0][1];
    std::cout << "vertex1_index " << vertex1_index << "\n";

    unsigned short vertex2_index = (unsigned short)arestas[0][2];
    std::cout << "vertex2_index " << vertex2_index << "\n";

    // Declare the variables for the occurrences of vertex1 and vertex2
    int vertex1_times = 0;
    int vertex2_times = 0;

    // Go through the "indices" vector to find the value of the two vertex and count their occurrences
    // percorre o vetor de indices para a contagem da ocorrencia de cada um dos vertices
    vertex1_times = std::count (indices.begin(), indices.end(), indices[vertex1_index]);
    std::cout << "vertex1_times " << vertex1_times << "\n";

    vertex2_times = std::count (indices.begin(), indices.end(), indices[vertex2_index]);
    std::cout << "vertex2_times " << vertex2_times << "\n";

    std::cout << "VERTEX 1: " << vertex1_index << "-" << vertex1_times << "\n";
    std::cout << "VERTEX 2: " << vertex2_index << "-" << vertex2_times << "\n";

    // Check which vertex has more occurrences - this will be removed
    // then replace all its occurrences for the value of the other vertex
    if (vertex1_times > vertex2_times)
    {
        std::replace(indices.begin(), indices.end(), indices[vertex1_index], indices[vertex2_index]);
    }
    else
    {
        std::replace(indices.begin(), indices.end(), indices[vertex2_index], indices[vertex1_index]);
    }

    // Check if there is any "line-triangles" - three connections between two vertex - and remove them
    for (int j = 0; j < indices.size(); j+=3) {
        if(indices[j] == indices[j+1] || indices[j] == indices[j+2] || indices[j+1] == indices[j+2])
        {
            // choose the same position (j) because "erase" resizes the vector after executes
            indices.erase(indices.begin() + j);
            indices.erase(indices.begin() + j);
            indices.erase(indices.begin() + j);
        }

    }

    arestas.clear();
}
