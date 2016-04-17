//
// Created by inessa on 12/04/16.
//

#ifndef MODEL_H
#define MODEL_H

#include<mesh.hpp>
#include<vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Model
{
    glm::mat4 modelMatrix;
    std::vector<glm::mat4> animatedMatrix;

    GLuint modelMatrixID;
    GLuint texture;
    GLuint textureID;

    Mesh * mesh;

    void activateTexture();
    void initializeMesh();

public:
    Model(char*, char*, GLuint, Mesh*);
    ~Model();

    // Getters
    glm::mat4 getModelMatrix();
    GLuint getModelMatrixID();
    GLuint getTexture();
    GLuint getTextureID();

    // Setters
    void setModelMatrix(glm::mat4);
    void setModelMatrixID(GLuint);
    void setTextureID(GLuint);

    void queueTransformation();
    void applyTransformation();
    void interpolateMatrix();
    void bindTexture();

    void draw();

};


#endif //CG_UFPEL_MODEL_H
