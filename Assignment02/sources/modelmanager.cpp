//
// Created by inessa on 20/04/16.
//
//
#include "modelmanager.hpp"
#include <shader.hpp>
#include <iostream>
#include <controls.hpp>
#include <glm/gtc/matrix_transform.hpp>

modelManager::modelManager(char* vertexShaders, char* fragmentShaders, char* lightPosition)
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    programID = LoadShaders(vertexShaders, fragmentShaders);
    lightID = glGetUniformLocation(programID, lightPosition);
    matrixID = glGetUniformLocation(programID, "MVP");
    viewMatrixID = glGetUniformLocation(programID, "V");

}

GLuint modelManager::getvertexArrayID()  {  return vertexArrayID; }

GLuint modelManager::getProgramID()     {   return programID;  }

void modelManager::generateMVP(Model &model)
{
    modelManager::projectionMatrix = getProjectionMatrix();
    modelManager::viewMatrix = getViewMatrix();
    modelManager::MVP = projectionMatrix * viewMatrix * model.getModelMatrix();
}

glm::mat4 modelManager::getprojectionMatrix()  {  return modelManager::projectionMatrix;  }

glm::mat4 modelManager::getviewMatrix()  {  return modelManager::viewMatrix;  }

GLuint modelManager::getMatrixID()  {  return modelManager::matrixID;  }

GLuint modelManager::getViewMatrixID() {    return modelManager::viewMatrixID;  }

void modelManager::setProjectionMatrix(glm::mat4 newProjectionMatrix)
{
    modelManager::projectionMatrix = newProjectionMatrix;
}

void modelManager::setViewMatrix(glm::mat4 newViewMatrix)
{
    modelManager::viewMatrix = newViewMatrix;
}

void modelManager::loadMesh(char* path)
{
    modelManager::meshes.push_back(Mesh(path));
}

void modelManager::setLightPosition(glm::vec3 lightPos = glm::vec3(4, 4, 4))
{
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
}

void modelManager::sendTransformation(Model &model)
{
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(model.getModelMatrixID(), 1, GL_FALSE, &model.getModelMatrix()[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
}


void modelManager::draw()
{
    for (int j = 0; j < models.size(); j++)
    {
        generateMVP(models[j]);
        sendTransformation(models[j]);
        models[j].draw();
    }

}

modelManager::~modelManager()
{

}

void modelManager::createModel(Mesh & mesh, char* texturePath, char * textureSampler)
{
    models.push_back(Model(texturePath, textureSampler, this->programID, &mesh));
}

GLuint modelManager::getLightID() {   return lightID;  }

void modelManager::setLightID(GLuint newID)  {  modelManager::lightID = newID;  }

void modelManager::setProgramID(GLuint newID) {  modelManager::programID = newID; }

void modelManager::setMatrixID(GLuint newID) {  modelManager::matrixID = newID; }

void modelManager::setViewMatrixID(GLuint newID) { modelManager::viewMatrixID = newID; }

void modelManager::unloadMesh(Mesh &mesh) {  mesh.unloadMesh(); }

void modelManager::assignMatrix(Model &) { }

std::vector<Mesh> *modelManager::getMeshes() {  return &meshes; }

std::vector<Model> *modelManager::getModels()
{
    return &models;
}

void modelManager::queueTransformation(int idModel, glm::mat4 transformationMatrix)
{
    models[idModel].queueTransformation(transformationMatrix);
}