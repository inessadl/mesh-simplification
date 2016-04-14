//
// Created by inessa on 12/04/16.
//

#include <objloader.hpp>
#include <vboindexer.hpp>
#include "mesh.hpp"


Mesh::Mesh(const char * path)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    // Load it into a VBO
    bool res = loadOBJ(path, vertices, uvs, normals);
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    // GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    // GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    // GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}


void Mesh::loadMesh()
{
    if (vertexBuffer != NULL)
    {
        // 1rst attribute buffer : vertices
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


    if (uvbuffer != NULL)
    {
        // 2nd attribute buffer : UVs
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


    if (normalbuffer != NULL)
    {
        // 3rd attribute buffer : normals
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

}

void Mesh::unloadMesh ()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

GLuint * Mesh::getVertexBuffer()    const   { return &vertexbuffer; }
GLuint * Mesh::getUvBuffer()        const   { return &uvbuffer; }
GLuint * Mesh::getNormalBuffer()    const   { return &normalbuffer; }
GLuint * Mesh::getElementBuffer()   const   { return &elementbuffer; }
GLuint * Mesh::getID()              const   { return; }

std::vector<glm::vec3> * Mesh::getIndexedVertices()   const   { return &indexed_vertices; }
std::vector<glm::vec2> * Mesh::getIndexedUvs()        const   { return &indexed_uvs; }
std::vector<glm::vec3> * Mesh::getIndexedNormals()    const   { return &indexed_normals; }
std::vector<unsigned short> * Mesh::getIndices()      const   { return &indices; }

void Mesh::setIndices(const std::vector<unsigned short> &indices)       {  Mesh::indices = indices; }
void Mesh::setIndexedVertices(const std::vector<glm::vec3> &vertices)   { Mesh::indexed_vertices = vertices; }
void Mesh::setIndexedUvs(const std::vector<glm::vec2> &uvs)             { Mesh::indexed_uvs = uvs; }
void Mesh::setIndexedNormals(const std::vector<glm::vec3> &normals)     { Mesh::indexed_normals = normals; }