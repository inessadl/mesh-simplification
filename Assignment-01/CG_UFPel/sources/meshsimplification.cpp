//
// Created by inessa on 01/04/16.
//

#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include <meshsimplification.hpp>
#include <iostream>


bool MeshSimplification::comp(const std::vector<float>  &a, const std::vector<float>  &b)
{
    return a[0] < b[0];
}

// recebe as 3 posições (ex. 0, 1 e 2) do vetor e
// calcula a distancia entre os pontos (arestas)
void MeshSimplification::removeVertex(std::vector<unsigned short> &indices, std::vector<glm::vec3> indexed_vertex)
{
    std::vector<std::vector<float> > arestas;
    std::vector<float> x;

    // calcula a menor aresta
    for (int i = 0; i < indices.size(); i+=3)
    {

        //calcula e adiciona arestas e seus vertices na matriz de arestas

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

    // ordena o vetor de arestas pela menor - primeira posição é a aresta a ser removida
    std::sort(arestas.begin(), arestas.end(), MeshSimplification::comp);

    // porém é preciso verificar qual dos dois vértices que conectam essa aresta será removido
    // o vértice a ser removido será aquele que possui mais conexões

    // percorrer o vetor de indices para encontrar os vertices e contar quantas vezes cada um deles aparece

    unsigned short vertex1_index = (unsigned short)arestas[0][1]; // atribui o valor encontrado na matriz de arestas - vertice 1
    std::cout << "vertex1_index " << vertex1_index << "\n";

    unsigned short vertex2_index = (unsigned short)arestas[0][2];
    std::cout << "vertex2_index " << vertex2_index << "\n";

    int vertex1_times = 0 ;  // número de vezes que o vértice 1 aparece no vetor de indices

    int vertex2_times = 0 ;

    // percorre o vetor de indices para a contagem da ocorrencia de cada um dos vertices
    vertex1_times = std::count (indices.begin(), indices.end(), indices[vertex1_index]);
    std::cout << "vertex1_times " << vertex1_times << "\n";

    vertex2_times = std::count (indices.begin(), indices.end(), indices[vertex2_index]);
    std::cout << "vertex2_times " << vertex2_times << "\n";

    std::cout << "VERTEX 1: " << vertex1_index << "-" << vertex1_times << "\n";
    std::cout << "VERTEX 2: " << vertex2_index << "-" << vertex2_times << "\n";

    // TODO criar um booleano para acessar o vértice que será removido
    // verifica vértice com a maior ocorrencia e retorna ele
    if (vertex1_times > vertex2_times)
    {
        std::replace(indices.begin(), indices.end(), indices[vertex1_index], indices[vertex2_index]);
    }
    else
    {
        std::replace(indices.begin(), indices.end(), indices[vertex2_index], indices[vertex1_index]);
    }

    for (int j = 0; j < indices.size(); j+=3) {
        if(indices[j] == indices[j+1] || indices[j] == indices[j+2] || indices[j+1] == indices[j+2])
        {
            indices.erase(indices.begin() + j);
            indices.erase(indices.begin() + j);
            indices.erase(indices.begin() + j);
        }

    }

    arestas.clear();
}

