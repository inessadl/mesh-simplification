//
// Created by inessa on 01/04/16.
//

#ifndef CG_UFPEL_MESHSIMPLIFICATION_H
#define CG_UFPEL_MESHSIMPLIFICATION_H

#include <stdio.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#endif //CG_UFPEL_MESHSIMPLIFICATION_H

#include <vector>


class MeshSimplification
{
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertex;

    // compare
    static bool comp(const std::vector<float> &, const std::vector<float> & );

public:
    void removeVertex(std::vector<unsigned short> &indices, std::vector<glm::vec3> indexed_vertex);

};