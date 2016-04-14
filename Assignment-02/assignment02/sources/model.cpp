//
// Created by inessa on 12/04/16.
//

#include "model.hpp"

Model::Model(char * texturePath, char * textureSampler, GLuint programID, int meshID)
{
    texture = loadDDS(texturePath);
    modelMatrixID = glGetUniformLocation(programID, "M");
    textureID = glGetUniformLocation(programID, textureSampler);

    Model::meshID = meshID;
}

Model::~Model()
{

}

// void Model::CleanBuffer()
// {
//     glDeleteBuffers(1, &vertexbuffer);
//     glDeleteBuffers(1, &uvbuffer);
//     glDeleteBuffers(1, &normalbuffer);
//     glDeleteBuffers(1, &elementbuffer);
// }
