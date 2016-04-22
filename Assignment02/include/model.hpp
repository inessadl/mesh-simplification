//
// Created by inessa on 20/04/16.
//

#ifndef ASSIGNMENT02_MODEL_HPP
#define ASSIGNMENT02_MODEL_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "mesh.hpp"

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

    Model(char *, char *, GLuint, Mesh*);
    ~Model();

    /* Getters */
    glm::mat4 getModelMatrix();
    GLuint getModelMatrixID();
    GLuint getTexture();
    GLuint getTextureID();

    /* Setters */
    void setModelMatrix(glm::mat4);
    void setModelMatrixID(GLuint);
    void setTextureID(GLuint);


    void queueTransformation();
    void applyTransformation();
    void interpolateMatrix();
    void draw();

};


#endif //ASSIGNMENT02_MODEL_H
