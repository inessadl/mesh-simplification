//
// Created by inessa on 20/04/16.
//
//

#include <objloader.hpp>
#include <vboindexer.hpp>
#include <GL/glew.h>
#include "mesh.hpp"


Mesh::Mesh(char * path)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    loadOBJ(path, vertices, uvs, normals);
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);


    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

Mesh::~Mesh()
{

}

// Getters
GLuint * Mesh::getVertexBuffer()    {  return &vertexbuffer;  }
GLuint * Mesh::getUvBuffer()        {  return &uvbuffer;      }
GLuint * Mesh::getNormalBuffer()    {  return &normalbuffer;  }
GLuint * Mesh::getElementBuffer()   {  return &elementbuffer; }

std::vector<glm::vec3> * Mesh::getIndexedVertices() {  return &indexed_vertices; }
std::vector<glm::vec2> * Mesh::getIndexedUvs()      {  return &indexed_uvs;      }
std::vector<glm::vec3> * Mesh::getIndexedNormals()  {  return &indexed_normals;  }
std::vector<unsigned short> * Mesh::getIndices()    {  return &indices;          }

// Setters
void Mesh::setIndexedVertices( std::vector<glm::vec3> &new_vertices)  {  Mesh::indexed_vertices = new_vertices; }
void Mesh::setIndexedUvs( std::vector<glm::vec2> &new_uvs)            {  Mesh::indexed_uvs = new_uvs;           }
void Mesh::setIndexedNormals( std::vector<glm::vec3> &new_normals)    {  Mesh::indexed_normals = new_normals;   }
void Mesh::setIndices( std::vector<unsigned short> &new_indices)      {  Mesh::indices = new_indices;           }

void Mesh::loadMesh()
{

    if(&vertexbuffer != NULL)
    {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );
    }

    if(&uvbuffer != NULL)
    {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );
    }


    if(&normalbuffer != NULL)
    {
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
}

void Mesh::unloadMesh()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}