//
// Created by inessa on 12/04/16.
//
#include <GL/glew.h>
#include <texture.hpp>
#include <model.hpp>
#include <mesh.hpp>

/* */
Model::Model(char * texturePath, char * textureSampler, GLuint programID, Mesh *mesh)
{
    texture = loadDDS(texturePath);
    modelMatrixID = glGetUniformLocation(programID, "M");
    textureID = glGetUniformLocation(programID, textureSampler);

    Model::mesh = mesh;

    modelMatrix = glm::mat4(1.0); // 4x4 matrix (identity matrix in this case)
}

/* Default destructor */
Model::~Model()
{

}

/* Getters */
glm::mat4 Model::getModelMatrix()   {   return Model::modelMatrix;  }
GLuint Model::getModelMatrixID()    {   return Model::modelMatrixID;  }
GLuint Model::getTexture()          {   return texture; }
GLuint Model::getTextureID()        {   return textureID; }

/* Setters */
void Model::setModelMatrix(glm::mat4 matrix)        {  Model::modelMatrix = matrix;  }
void Model::setModelMatrixID(GLuint modelMatrixID)  {  modelMatrixID = modelMatrixID;  }
void Model::setTextureID(GLuint textureID)          {  textureID = textureID; }


void Model::activateTexture()
{
  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Model::texture);
  glUniform1i(Model::textureID, 0);   // set our texture to user Texture Unit 0
}

/* Calls the method to load a mesh into the buffer */
void Model::initializeMesh()
{
  mesh->loadMesh();
}

/* Draw the elements after activates texture and initialize the mesh */
void Model::draw()
{
  Model::activateTexture();
  Model::initializeMesh();

  glDrawElements(
    GL_TRIANGLES,                 // mode
    mesh->getIndices()->size(),   // count
    GL_UNSIGNED_SHORT,            // type
    (void*)0                      // element array buffer offset
  );
}
