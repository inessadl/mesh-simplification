//
// Created by inessa on 12/04/16.
//

#ifndef CG_UFPEL_MESH_H
#define CG_UFPEL_MESH_H

#include <glm/glm.hpp>
#include <vector>


class Mesh
{

private:
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
//    GLuint ModelMatrixID;


    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;

public:
    Mesh(const char *);
    ~Mesh();

    // Getters
    GLuint * getVertexBuffer() const;
    GLuint * getUvBuffer() const;
    GLuint * getNormalBuffer() const;
    GLuint * getElementBuffer() const;
    GLuint * getID() const;

    std::vector<glm::vec3> * getIndexedVertices() const;
    std::vector<glm::vec2> * getIndexedUvs() const;
    std::vector<glm::vec3> * getIndexedNormals() const;
    std::vector<unsigned short> * getIndices() const;

    // Setters
    void setIndices(const std::vector<unsigned short> &);
    void setIndexedVertices(const std::vector<glm::vec3> &);
    void setIndexedUvs(const std::vector<glm::vec2> &);
    void setIndexedNormals(const std::vector<glm::vec3> &);

    void loadMesh();
    void unloadMesh();

    // Bind Buffers

    //void bindBufferChannel(int);
    //	void CleanBuffer();


};




#endif //CG_UFPEL_MESH_H
