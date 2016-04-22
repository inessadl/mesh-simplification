//
// Created by inessa on 20/04/16.
//

#include "modelmanager.hpp"
#include <shader.hpp>
#include <iostream>
#include <controls.hpp>
#include <glm/gtc/matrix_transform.hpp>

ModelManager::ModelManager(char * vertexShaders, char * fragmentShaders, char * lightPosition)
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    programID = LoadShaders(vertexShaders, fragmentShaders);
    lightID = glGetUniformLocation(programID, lightPosition);
    matrixID = glGetUniformLocation(programID, "MVP");
    viewMatrixID = glGetUniformLocation(programID, "V");

}

ModelManager::~ModelManager()
{

}

GLuint ModelManager::getvertexArrayID()   {  return vertexArrayID;  }
GLuint ModelManager::getProgramID()       {  return programID;  }

void ModelManager::generateMVP (Model &model)
{
    ModelManager::projectionMatrix = getProjectionMatrix();
    ModelManager::viewMatrix = getViewMatrix();
    ModelManager::MVP = projectionMatrix * viewMatrix * model.getModelMatrix();

}

/* Getters */
glm::mat4 ModelManager::getprojectionMatrix()   {  return projectionMatrix;  }
glm::mat4 ModelManager::getViewMatrix()         {  return viewMatrix; }

GLuint ModelManager::getMatrixID()              {  return matrixID;  }
GLuint ModelManager::getViewMatrixID()          {  return viewMatrixID; }

/* Setters */
void ModelManager::setprojectionMatrix(glm::mat4 projectionMatrix)  {  ModelManager::projectionMatrix = projectionMatrix; }
void ModelManager::setViewMatrix(glm::mat4 viewMatrix)              {  ModelManager::viewMatrix = viewMatrix; }

void ModelManager::loadMesh(char * path)
{
    ModelManager::meshes.push_back(Mesh(path));
}

GLuint ModelManager::getLightID()               {  return lightID;  }
std::vector<Mesh> *ModelManager::getMeshes()    {  return &meshes; }
std::vector<Model> *ModelManager::getModels()   {  return &models; }

void ModelManager::setLightID(GLuint id)        {  ModelManager::lightID = id; }
void ModelManager::setProgramID(GLuint id )     {  ModelManager::programID = id; }
void ModelManager::setMatrixID(GLuint id )      {  ModelManager::matrixID = id; }
void ModelManager::setViewMatrixID(GLuint id )  {  ModelManager::viewMatrixID = id; }


void ModelManager::setLightPosition(glm::vec3 lightPos = glm::vec3(4,4,4))
{
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
}

void ModelManager::createModel(Mesh & mesh, char * texturePath, char * textureSampler)
{
    models.push_back(Model(texturePath, textureSampler, this->programID, &mesh));
}

void ModelManager::unloadMesh(Mesh & mesh) {  mesh.unloadMesh(); }

void ModelManager::assignMatrix(Model &)
{

}

void ModelManager::draw()
{
    models[0].setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(-1.0f, 0.0f, -3.0f)));
    models[1].setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 0.0f, 3.0f)));

    for (int j = 0; j < models.size(); j++)
    {
        generateMVP(models[j]);
        sendTransformation(models[j]);
        models[j].draw();

    }
}

void ModelManager::sendTransformation(Model & model)
{
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(model.getModelMatrixID(), 1, GL_FALSE, &model.getModelMatrix()[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
}



