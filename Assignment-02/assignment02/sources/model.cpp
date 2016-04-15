//
// Created by inessa on 12/04/16.
//

#include <texture.hpp>
#include "model.hpp"

Model::Model(char * texturePath, char * textureSampler, GLuint programID, int meshID)
{
    texture = loadDDS(texturePath);
    modelMatrixID = glGetUniformLocation(programID, "M");
    textureID = glGetUniformLocation(programID, textureSampler);

    Model::meshID = meshID;

    // matrix 4 x 4
    modelMatrix = glm::mat4(1.0);
}

Model::~Model()
{
    glDeleteTextures(1, &texture);
}

glm::mat4 Model::getModelMatrix() 
{
    return Model::modelMatrix;
}

GLuint Model::getModelMatrixID() 
{
    return Model::modelMatrixID;
}

GLuint Model::getTexture() 
{
    return texture;
}

GLuint Model::getTextureID() 
{
    return textureID;
}

GLuint Model::getMeshID() 
{
    return meshID;
}

void Model::setModelMatrixID(GLuint newModelMatrixID)
{
    modelMatrixID = newModelMatrixID;
}

void Model::setTextureID(GLuint newTextureID)
{
    textureID = newTextureID;
}



// void Model::CleanBuffer()
// {
//     glDeleteBuffers(1, &vertexbuffer);
//     glDeleteBuffers(1, &uvbuffer);
//     glDeleteBuffers(1, &normalbuffer);
//     glDeleteBuffers(1, &elementbuffer);
// }
