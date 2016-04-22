//
// Created by inessa on 20/04/16.
//

#include <GL/glew.h>
#include <model.hpp>
#include <texture.hpp>
#include "model.hpp"

Model::Model(char * texturePath, char * textureSampler, GLuint programID, Mesh *mesh)
{
    texture = loadDDS(texturePath);
    modelMatrixID = glGetUniformLocation(programID, "M");
    textureID = glGetUniformLocation(programID, textureSampler);

    Model::mesh = mesh;

    modelMatrix = glm::mat4(1.0);

}

Model::~Model()
{

}

/* Getters */
glm::mat4 Model::getModelMatrix()   {  return Model::modelMatrix; }
GLuint Model::getModelMatrixID()    {  return Model::modelMatrixID; }
GLuint Model::getTexture()          {  return texture; }
GLuint Model::getTextureID()        {  return textureID; }

/* Setters */
void Model::setModelMatrix(glm::mat4 matrix)        {  Model::modelMatrix = matrix; }
void Model::setModelMatrixID(GLuint modelMatrixID)  {  Model::modelMatrixID = modelMatrixID; }
void Model::setTextureID(GLuint textureId)          {  textureID = textureId; }


void Model::activateTexture()
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Model::texture);
    // Set our texture sampler to user Texture Unit 0
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
            GL_TRIANGLES,                 // mode
            mesh->getIndices()->size(),   // count
            GL_UNSIGNED_SHORT,            // type
            (void*)0
    );
}