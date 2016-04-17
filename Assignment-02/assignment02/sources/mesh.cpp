//
// Created by inessa on 12/04/16.
//

#include <objloader.hpp>
#include <vboindexer.hpp>
#include <GL/glew.h>
#include "mesh.hpp"

/* Receives the path to the mesh that will be loaded */
Mesh::Mesh(char * path)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    // Load it into a VBO
    loadOBJ(path, vertices, uvs, normals);
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

    // GLuint elementbuffer - a buffer for the indices
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}

/* Default destructor */
Mesh::~Mesh ()
{

}

/* Getters */
GLuint * Mesh::getVertexBuffer()    {   return &vertexbuffer;   }
GLuint * Mesh::getUvBuffer()        {   return &uvbuffer;   }
GLuint * Mesh::getNormalBuffer()    {   return &normalbuffer;   }
GLuint * Mesh::getElementBuffer()   {   return &elementbuffer;  }

std::vector<unsigned short> * Mesh::getIndices()      {   return &indices;  }
std::vector<glm::vec3> * Mesh::getIndexedVertices()   {   return &indexed_vertices;  }
std::vector<glm::vec2> * Mesh::getIndexedUvs()        {   return &indexed_uvs;  }
std::vector<glm::vec3> * Mesh::getIndexedNormals()    {   return &indexed_normals;  }


/* Setters */
void Mesh::setIndices(std::vector<unsigned short> &indices)       {   Mesh::indices = indices;  }

void Mesh::setIndexedVertices(std::vector<glm::vec3> &vertices)   {   Mesh::indexed_vertices = vertices; }
void Mesh::setIndexedUvs( std::vector<glm::vec2> &uvs)            {   Mesh::indexed_uvs = uvs;  }
void Mesh::setIndexedNormals( std::vector<glm::vec3> &normals)    {   Mesh::indexed_normals = normals;  }

/* Loads the mesh to GPU */
void Mesh::loadMesh()
{
    if (&vertexbuffer != NULL)
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


    if (&uvbuffer != NULL)
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


    if (&normalbuffer != NULL)
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

/* Unloads the mesh from GPU */
void Mesh::unloadMesh ()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
