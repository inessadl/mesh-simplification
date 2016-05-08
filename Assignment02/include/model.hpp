//
// Created by inessa on 20/04/16.
//

#ifndef ASSIGNMENT02_MODEL_HPP
#define ASSIGNMENT02_MODEL_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "mesh.hpp"
#include <AntTweakBar.h>

class Model
{
    glm::mat4 modelMatrix;
    std::vector<glm::mat4> animatedMatrix;
    GLuint modelMatrixID;
    GLuint texture;
    GLuint textureID;
    Mesh * mesh;
    std::vector<glm::mat4> transformationQueue;

    void activateTexture();
    void initializeMesh();
    void applyTransformation();

public:

    Model(char*, char*, GLuint, Mesh*);
    ~Model();

    // Getters
    glm::mat4 getModelMatrix();
    GLuint getModelMatrixID();
    GLuint getTexture();
    GLuint getTextureID();

    // Setters
    void setModelMatrixID(GLuint);
    void setTextureID(GLuint);

    void queueTransformation(glm::mat4);
    void interpolateMatrix();
    void draw();
    void TW_CALL transformation(void*);
};

#endif