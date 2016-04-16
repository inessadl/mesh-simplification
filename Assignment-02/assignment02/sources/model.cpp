//
// Created by inessa on 12/04/16.
//
#include <GL/glew.h>
#include <texture.hpp>
#include <model.hpp>
#include <mesh.hpp>

Model::Model(char * texturePath, char * textureSampler, GLuint programID, Mesh *mesh)
{
    texture = loadDDS(texturePath);
    modelMatrixID = glGetUniformLocation(programID, "M");
    textureID = glGetUniformLocation(programID, textureSampler);

    Model::mesh = mesh;

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

// GLuint Model::getMeshID()
// {
//     return meshID;
// }
void Model::setModelMatrix(glm::mat4 matrix)
{
  Model::modelMatrix = matrix;
}

void Model::setModelMatrixID(GLuint modelMatrixID)
{
    modelMatrixID = modelMatrixID;
}


void Model::activateTexture()
{
  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Model::texture);
  // Set our texture to user Texture Unit 0
  glUniform1i(Model::textureID, 0);
}

void Model::initializeMesh()
{
  mesh->loadMesh();
}

void Model::draw()
{
  Model::activateTexture();
  Model::initializeMesh();
  glDrawElements(
    GL_TRIANGLES,               // mode
    mesh->getIndices()->size(), // count
    GL_UNSIGNED_SHORT,          // type
    (void*)0                    // element array buffer offset
  );
}

void Model::setTextureID(GLuint textureID)
{
    textureID = textureID;
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
