//
// Created by inessa on 20/04/16.
//

#ifndef ASSIGNMENT02_MESH_HPP
#define ASSIGNMENT02_MESH_HPP


#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class Mesh
{
private:
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;

public:
    Mesh( char *);
    ~Mesh();

    //Getters
    GLuint * getVertexBuffer();
    GLuint * getUvBuffer();
    GLuint * getNormalBuffer();
    GLuint * getElementBuffer();
    std::vector<glm::vec3> * getIndexedVertices() ;
    std::vector<glm::vec2> * getIndexedUvs() ;
    std::vector<glm::vec3> * getIndexedNormals() ;
    std::vector<unsigned short> * getIndices() ;


    //Setters
    void setIndices( std::vector<unsigned short> &);
    void setIndexedVertices( std::vector<glm::vec3> &);
    void setIndexedUvs( std::vector<glm::vec2> &);
    void setIndexedNormals( std::vector<glm::vec3> &);

    //Bind Buffers
    void loadMesh();
    void unloadMesh();

};


#endif //COMPGRAPHIC_MESH_HPP